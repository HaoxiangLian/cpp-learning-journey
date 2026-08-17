#!/usr/bin/env python3
"""Validate course structure, release state, links, and safety invariants."""

from __future__ import annotations

import json
import pathlib
import re
import sys
from urllib.parse import unquote


ROOT = pathlib.Path(__file__).resolve().parents[1]
REQUIRED_DAY_HEADINGS = (
    "## 🎯 今日攻坚目标",
    "## 🔁 前置知识检查",
    "## 📖 核心知识重构",
    "## 💻 最小可运行示例",
    "## 🐛 错误代码诊断",
    "## 🎤 高频面试实战",
    "## ✍️ 当日练习",
    "## 🧠 深度思考题",
    "## ✅ 完成标准",
)
BANNED_DAY_HEADINGS = ("## 📚 今日术语卡",)
CORE_MECHANISM_PATTERN = re.compile(r"^### 机制(?:[一二三四]|[1-4])[:：].+$", re.MULTILINE)
REQUIRED_MECHANISM_MARKERS = (
    "**实际问题**",
    "**概念落点**",
    "**代码与机制**",
    "**错误做法与修复**",
    "**小检查**",
)
CONTROLLED_PATHS = (
    "README.md",
    "ROADMAP_24_DAYS.md",
    "LEARNING_PLAN.md",
    "PROGRESS.md",
    "progress.md",
    "AGENTS.md",
    "CMakeLists.txt",
    "days",
    "docs",
    "examples",
    "exercises",
    "solutions",
    "interview",
    "release",
    "scripts",
    ".github",
)
LINK_PATTERN = re.compile(r"(?<!!)\[[^\]]+\]\(([^)]+)\)")
GLOSSARY_FILE_PATTERN = re.compile(r"day(\d{2})\.md")
GLOSSARY_TERM_PATTERN = re.compile(r"^### (.+)$", re.MULTILINE)
REQUIRED_GLOSSARY_MARKERS = (
    "- **定义**",
    "- **决定什么**",
    "- **不决定什么**",
    "- **最小示例",
    "- **常见误解**",
)
INTERVIEW_ANSWER_FILE_PATTERN = re.compile(r"day(\d{2})\.md")
INTERVIEW_QUESTION_PATTERN = re.compile(r"^## (Q\d{3})$", re.MULTILINE)
INTERVIEW_FOLLOWUP_PATTERN = re.compile(r"^#### 追问 \d+：(.+)$", re.MULTILINE)
REQUIRED_INTERVIEW_ANSWER_MARKERS = (
    "**主问题**",
    "### 30 秒回答",
    "### 完整答题逻辑",
    "### 连续追问与参考答案",
    "### 容易失分",
)


def fail(errors: list[str], message: str) -> None:
    errors.append(message)


def iter_controlled_files() -> list[pathlib.Path]:
    files: list[pathlib.Path] = []
    for name in CONTROLLED_PATHS:
        path = ROOT / name
        if path.is_file():
            files.append(path)
        elif path.is_dir():
            files.extend(item for item in path.rglob("*") if item.is_file())
    return files


def validate_links(errors: list[str], markdown: pathlib.Path) -> None:
    text = markdown.read_text(encoding="utf-8")
    for raw_target in LINK_PATTERN.findall(text):
        target = raw_target.strip().split()[0].strip("<>")
        if target.startswith(("http://", "https://", "mailto:", "#")):
            continue
        path_part = unquote(target.split("#", 1)[0].split("?", 1)[0])
        if not path_part:
            continue
        resolved = (markdown.parent / path_part).resolve()
        try:
            resolved.relative_to(ROOT)
        except ValueError:
            fail(errors, f"link escapes repository: {markdown.relative_to(ROOT)} -> {target}")
            continue
        if not resolved.exists():
            fail(errors, f"broken link: {markdown.relative_to(ROOT)} -> {target}")


def validate_glossary(errors: list[str], published: list[int]) -> None:
    index = ROOT / "docs" / "glossary.md"
    index_text = index.read_text(encoding="utf-8")
    if re.search(r"^### ", index_text, flags=re.MULTILINE) or "- **定义**" in index_text:
        fail(errors, "docs/glossary.md must be navigation only, not contain formal term entries")
    index_days = re.findall(r"^\| (\d{2}) \|", index_text, flags=re.MULTILINE)
    if index_days != [f"{day:02d}" for day in range(1, 25)]:
        fail(errors, "glossary index must contain exactly ordered day rows 01 through 24")

    glossary_dir = ROOT / "docs" / "glossary"
    glossary_files = sorted(glossary_dir.glob("*.md")) if glossary_dir.exists() else []
    unrecognized = [path.name for path in glossary_files if GLOSSARY_FILE_PATTERN.fullmatch(path.name) is None]
    if unrecognized:
        fail(errors, "unrecognized glossary files: " + ", ".join(unrecognized))

    actual_days = [int(GLOSSARY_FILE_PATTERN.fullmatch(path.name).group(1)) for path in glossary_files]
    if actual_days != published:
        fail(errors, f"glossary day files must exactly match published_days: {published}; found {actual_days}")

    term_owners: dict[str, pathlib.Path] = {}
    for day, path in zip(actual_days, glossary_files):
        relative = path.relative_to(ROOT)
        text = path.read_text(encoding="utf-8")
        code = f"day{day:02d}"
        if f"(../../days/{code}.md)" not in text or "(../glossary.md)" not in text:
            fail(errors, f"{relative} must link to its lecture and the glossary index")
        if f"(glossary/{code}.md)" not in index_text:
            fail(errors, f"docs/glossary.md must link published glossary file {code}.md")

        terms = list(GLOSSARY_TERM_PATTERN.finditer(text))
        if not terms:
            fail(errors, f"{relative} must contain at least one formal term entry")
        for term_index, term in enumerate(terms):
            section_end = terms[term_index + 1].start() if term_index + 1 < len(terms) else len(text)
            section = text[term.start():section_end]
            heading = term.group(1).strip()
            normalized = re.sub(r"\s+", "", heading).casefold()
            previous = term_owners.get(normalized)
            if previous is not None:
                fail(errors, f"duplicate glossary term '{heading}' in {previous.relative_to(ROOT)} and {relative}")
            else:
                term_owners[normalized] = path
            for marker in REQUIRED_GLOSSARY_MARKERS:
                if marker not in section:
                    fail(errors, f"{relative} term '{heading}' missing marker: {marker}")


def validate_interview_answers(errors: list[str], published: list[int]) -> None:
    answer_dir = ROOT / "interview" / "answers"
    index = answer_dir / "README.md"
    if not index.exists():
        fail(errors, "missing required path: interview/answers/README.md")
        return

    index_text = index.read_text(encoding="utf-8")
    index_days = re.findall(r"^\| (\d{2}) \|", index_text, flags=re.MULTILINE)
    if index_days != [f"{day:02d}" for day in range(1, 25)]:
        fail(errors, "interview answer index must contain exactly ordered day rows 01 through 24")

    answer_files = sorted(answer_dir.glob("day*.md"))
    unrecognized = [
        path.name for path in answer_files
        if INTERVIEW_ANSWER_FILE_PATTERN.fullmatch(path.name) is None
    ]
    if unrecognized:
        fail(errors, "unrecognized interview answer files: " + ", ".join(unrecognized))

    actual_days = [
        int(INTERVIEW_ANSWER_FILE_PATTERN.fullmatch(path.name).group(1))
        for path in answer_files
    ]
    if actual_days != published:
        fail(
            errors,
            f"interview answer day files must exactly match published_days: {published}; "
            f"found {actual_days}",
        )

    question_ids: list[str] = []
    question_bank = (ROOT / "interview" / "question-bank.md").read_text(encoding="utf-8")
    for day, path in zip(actual_days, answer_files):
        relative = path.relative_to(ROOT)
        text = path.read_text(encoding="utf-8")
        code = f"day{day:02d}"
        if f"(../../days/{code}.md)" not in text or "(../question-bank.md)" not in text:
            fail(errors, f"{relative} must link to its lecture and the interview question index")
        if f"({code}.md)" not in index_text:
            fail(errors, f"interview/answers/README.md must link published answer file {code}.md")
        if f"(answers/{code}.md#" not in question_bank:
            fail(errors, f"interview/question-bank.md must link question answers for {code}.md")

        questions = list(INTERVIEW_QUESTION_PATTERN.finditer(text))
        if len(questions) != 2:
            fail(errors, f"{relative} must contain exactly two interview question sections")
        answer_followups_by_question: list[list[str]] = []
        for question_index, question in enumerate(questions):
            section_end = (
                questions[question_index + 1].start()
                if question_index + 1 < len(questions)
                else len(text)
            )
            section = text[question.start():section_end]
            question_id = question.group(1)
            question_ids.append(question_id)
            for marker in REQUIRED_INTERVIEW_ANSWER_MARKERS:
                if marker not in section:
                    fail(errors, f"{relative} {question_id} missing marker: {marker}")
            followups = list(INTERVIEW_FOLLOWUP_PATTERN.finditer(section))
            if not followups:
                fail(errors, f"{relative} {question_id} must contain at least one follow-up answer")
            answer_followups_by_question.append([item.group(1).strip() for item in followups])
            for followup_index, followup in enumerate(followups):
                followup_end = (
                    followups[followup_index + 1].start()
                    if followup_index + 1 < len(followups)
                    else len(section)
                )
                followup_section = section[followup.start():followup_end]
                if "**参考答案**" not in followup_section:
                    fail(
                        errors,
                        f"{relative} {question_id} follow-up {followup_index + 1} missing answer",
                    )

        lecture = ROOT / "days" / f"{code}.md"
        if lecture.exists() and len(answer_followups_by_question) == 2:
            lecture_text = lecture.read_text(encoding="utf-8")
            interview_start = lecture_text.find("## 🎤 高频面试实战")
            interview_end = lecture_text.find("## ✍️ 当日练习", interview_start + 1)
            interview_text = lecture_text[interview_start:interview_end]
            lecture_questions = list(re.finditer(r"^### 题 [12]：.+$", interview_text, re.MULTILINE))
            if len(lecture_questions) != 2:
                fail(errors, f"{lecture.relative_to(ROOT)} must contain exactly two interview questions")
            else:
                for question_index, lecture_question in enumerate(lecture_questions):
                    lecture_section_end = (
                        lecture_questions[question_index + 1].start()
                        if question_index + 1 < len(lecture_questions)
                        else len(interview_text)
                    )
                    lecture_section = interview_text[lecture_question.start():lecture_section_end]
                    followup_start = lecture_section.find("**可能连续追问**")
                    followup_end = lecture_section.find("**容易失分的说法**", followup_start + 1)
                    lecture_followups = re.findall(
                        r"^- (.+)$",
                        lecture_section[followup_start:followup_end],
                        flags=re.MULTILINE,
                    )
                    normalize = lambda value: value.replace("`", "").strip()
                    if [normalize(item) for item in lecture_followups] != [
                        normalize(item) for item in answer_followups_by_question[question_index]
                    ]:
                        fail(
                            errors,
                            f"{relative} {questions[question_index].group(1)} follow-ups must "
                            f"exactly match the lecture",
                        )

    expected_ids = [f"Q{number:03d}" for number in range(1, len(published) * 2 + 1)]
    if question_ids != expected_ids:
        fail(errors, f"interview question IDs must be sequential: {expected_ids}; found {question_ids}")


def main() -> int:
    errors: list[str] = []

    required = (
        "README.md",
        "ROADMAP_24_DAYS.md",
        "docs/glossary.md",
        "docs/lecture-writing-guide.md",
        "interview/question-bank.md",
        "interview/answers/README.md",
        "progress.md",
        "release/state.json",
        "release/manifest.json",
        "CMakeLists.txt",
    )
    for name in required:
        if not (ROOT / name).exists():
            fail(errors, f"missing required path: {name}")

    roadmap = (ROOT / "ROADMAP_24_DAYS.md").read_text(encoding="utf-8")
    day_rows = re.findall(r"^\| (\d{2}) \|", roadmap, flags=re.MULTILINE)
    if day_rows != [f"{day:02d}" for day in range(1, 25)]:
        fail(errors, "roadmap must contain exactly ordered day rows 01 through 24")

    state = json.loads((ROOT / "release/state.json").read_text(encoding="utf-8"))
    manifest = json.loads((ROOT / "release/manifest.json").read_text(encoding="utf-8"))
    published = state.get("published_days")
    if not isinstance(published, list) or published != sorted(set(published)):
        fail(errors, "published_days must be a sorted unique list")
        published = []
    if any(not isinstance(day, int) or not 1 <= day <= 24 for day in published):
        fail(errors, "published_days values must be integers in 1..24")

    if state.get("status") == "completed":
        if published != list(range(1, 25)) or state.get("next_day") is not None:
            fail(errors, "completed state requires days 1..24 and next_day null")
    else:
        expected_next = len(published) + 1
        if state.get("next_day") != expected_next:
            fail(errors, f"next_day must be {expected_next}")

    manifest_days = manifest.get("days", [])
    if [entry.get("day") for entry in manifest_days] != list(range(1, 25)):
        fail(errors, "manifest must contain exactly days 1 through 24")
    for entry in manifest_days:
        expected_status = "published" if entry.get("day") in published else "pending"
        if entry.get("status") != expected_status:
            fail(errors, f"manifest day {entry.get('day')} must be {expected_status}")

    validate_glossary(errors, published)
    validate_interview_answers(errors, published)

    for day in published:
        code = f"day{day:02d}"
        lecture = ROOT / "days" / f"{code}.md"
        for path in (
            lecture,
            ROOT / "docs" / "glossary" / f"{code}.md",
            ROOT / "examples" / code,
            ROOT / "exercises" / code / "README.md",
            ROOT / "solutions" / code / "README.md",
            ROOT / "interview" / "answers" / f"{code}.md",
        ):
            if not path.exists():
                fail(errors, f"published day {day} missing {path.relative_to(ROOT)}")
        if lecture.exists():
            lecture_text = lecture.read_text(encoding="utf-8")
            for heading in REQUIRED_DAY_HEADINGS:
                if heading not in lecture_text:
                    fail(errors, f"{lecture.relative_to(ROOT)} missing heading: {heading}")
            for heading in BANNED_DAY_HEADINGS:
                if heading in lecture_text:
                    fail(errors, f"{lecture.relative_to(ROOT)} uses banned standalone heading: {heading}")
            expected_glossary_prefix = f"../docs/glossary/{code}.md#"
            if expected_glossary_prefix not in lecture_text:
                fail(
                    errors,
                    f"{lecture.relative_to(ROOT)} must reference its day-specific formal glossary entries",
                )
            if "../docs/glossary.md#" in lecture_text:
                fail(errors, f"{lecture.relative_to(ROOT)} must not link formal terms to the glossary index")
            expected_answer_link = f"../interview/answers/{code}.md"
            if expected_answer_link not in lecture_text:
                fail(errors, f"{lecture.relative_to(ROOT)} must link its day-specific interview answers")

            core_start = lecture_text.find("## 📖 核心知识重构")
            core_end = lecture_text.find("## 💻 最小可运行示例", core_start + 1)
            if core_start >= 0 and core_end > core_start:
                core_text = lecture_text[core_start:core_end]
                mechanisms = list(CORE_MECHANISM_PATTERN.finditer(core_text))
                if not 2 <= len(mechanisms) <= 4:
                    fail(
                        errors,
                        f"{lecture.relative_to(ROOT)} must contain 2..4 core mechanisms; "
                        f"found {len(mechanisms)}",
                    )
                for index, mechanism in enumerate(mechanisms):
                    section_end = mechanisms[index + 1].start() if index + 1 < len(mechanisms) else len(core_text)
                    section = core_text[mechanism.start():section_end]
                    for marker in REQUIRED_MECHANISM_MARKERS:
                        if marker not in section:
                            fail(
                                errors,
                                f"{lecture.relative_to(ROOT)} {mechanism.group(0)} "
                                f"missing integrated marker: {marker}",
                            )

    legacy = list((ROOT / "days").glob("第*.md")) + list((ROOT / "exercises").glob("day-*.md"))
    if legacy:
        fail(errors, "legacy active-course files remain: " + ", ".join(str(p.relative_to(ROOT)) for p in legacy))

    controlled_files = iter_controlled_files()
    for path in controlled_files:
        if path.suffix.lower() in {".md", ".cpp", ".hpp", ".py", ".yml", ".yaml", ".json", ".txt"} or path.name == "CMakeLists.txt":
            text = path.read_text(encoding="utf-8")
            if re.search(r"^(<<<<<<<|=======|>>>>>>>)", text, flags=re.MULTILINE):
                fail(errors, f"merge conflict marker in {path.relative_to(ROOT)}")
        if path.suffix.lower() == ".md":
            validate_links(errors, path)

    cmake = (ROOT / "CMakeLists.txt").read_text(encoding="utf-8")
    for broken in (ROOT / "exercises").glob("day*/broken/*.cpp"):
        banner = broken.read_text(encoding="utf-8").splitlines()[:2]
        if not any("INTENTIONALLY INVALID" in line for line in banner):
            fail(errors, f"missing intentional-error banner: {broken.relative_to(ROOT)}")
    if re.search(r"add_(?:executable|library)\([^)]*broken/", cmake, flags=re.DOTALL):
        fail(errors, "an intentionally invalid source is included in a normal build target")

    if errors:
        print("repository validation failed:")
        for error in errors:
            print(f"- {error}")
        return 1

    print(f"repository validation passed: {len(controlled_files)} controlled files")
    return 0


if __name__ == "__main__":
    sys.exit(main())
