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


def main() -> int:
    errors: list[str] = []

    required = (
        "README.md",
        "ROADMAP_24_DAYS.md",
        "docs/glossary.md",
        "docs/lecture-writing-guide.md",
        "interview/question-bank.md",
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

    for day in published:
        code = f"day{day:02d}"
        lecture = ROOT / "days" / f"{code}.md"
        for path in (
            lecture,
            ROOT / "examples" / code,
            ROOT / "exercises" / code / "README.md",
            ROOT / "solutions" / code / "README.md",
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
            if "../docs/glossary.md#" not in lecture_text:
                fail(errors, f"{lecture.relative_to(ROOT)} must reference formal glossary entries")

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
