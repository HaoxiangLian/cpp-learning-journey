# Repository maintenance rules

These rules apply to every automated or manual course update.

## Course invariants

- Simplified Chinese prose, C++ identifiers and standard terms in English.
- C++17 is the primary standard. Mark C++11/14 origins and C++20 additions explicitly.
- Follow `docs/lecture-writing-guide.md`: do not create a standalone daily term-card section. Introduce a new term inside the relevant core mechanism, repeat its canonical one-sentence definition, and link the exact `docs/glossary/dayXX.md` anchor at first use.
- `docs/glossary.md` is navigation only. `docs/glossary/dayXX.md` owns the canonical definitions first introduced on that day. Before adding a term, search every file under `docs/glossary/`; if a definition changes, audit its first-use lecture, exercises, and interview answers together.
- Introduce at most five major concepts per day. Do not use an unexplained technical term to define a new term.
- Write for a learner whose C++ foundation is weak. Before a formal definition, show a concrete code state, execution step, or before/after table that answers “which object”, “what changed”, and “when it stops being usable”.
- Raise the abstraction level only after the learner can predict the current example. Phrases such as “language semantics”, “implementation model”, “observable behavior”, “ABI”, “register”, “optimizer”, and “allocation mechanism” must be translated in place; they must not carry the first explanation of a concept.
- Treat implementation boundaries as a second-pass supplement. Removing the implementation paragraph must not remove the rule needed to read, write, or diagnose the current C++ code.
- Separate standard guarantees from common compiler, OS, ABI, stack/heap, vptr, and vtable implementations.
- Never equate undefined behavior with a guaranteed crash or segmentation fault.
- Intentionally invalid code belongs under `exercises/dayXX/broken/`, carries an `INTENTIONALLY INVALID` banner, and is excluded from normal CMake targets.
- Do not modify `.obsidian/` as part of course publication.

## Required daily files

- `days/dayXX.md`
- `docs/glossary/dayXX.md` containing only terms first introduced that day
- `examples/dayXX/` with at least one independently runnable C++17 example
- `exercises/dayXX/README.md` without answers
- `solutions/dayXX/README.md` and solution code
- `interview/answers/dayXX.md` containing the two main interview answers and answers to every listed follow-up
- updates, only when needed, to the glossary index `docs/glossary.md`, interview answer index `interview/answers/README.md`, `interview/question-bank.md`, `CMakeLists.txt`, `progress.md`, `release/manifest.json`, and `release/state.json`

## Publication protocol

1. Read `release/state.json` from the default branch.
2. If `status` is `completed`, stop without writing.
3. Use exactly `next_day`; if its day file is already present and the state disagrees, stop and report the inconsistency.
4. A scheduled run must compare the manifest's `scheduled_date` for `next_day` with the current date in the state's timezone. If the date is still in the future, stop without writing. An explicit user request to “开始第 X 天学习” is a manual early-release override only when X equals `next_day`.
5. Create one branch named `release/dayXX-YYYY-MM-DD` from the current default-branch head.
6. Generate only that day's material, including `interview/answers/dayXX.md`, and the allowed indexes/state files. Update `interview/answers/README.md` so the newly published day becomes clickable.
7. Run the repository validator and GCC/Clang C++17 builds through CI. Do not merge on failure.
8. Merge only after required checks pass. Then append the day to `published_days`, advance `next_day` by one, and record the commit/PR in state.
9. After day 24, set `status` to `completed`, `next_day` to `null`, and never create day 25.

Repeated runs must be idempotent: an existing published day, branch, or PR is resumed or reported, never duplicated.

## Validation checklist

- Correct examples compile with warnings enabled on GCC and Clang.
- Relevant resource/lifetime examples run under AddressSanitizer and UndefinedBehaviorSanitizer.
- Markdown relative links resolve; conflict markers are absent.
- Each daily lecture has every required section and the exercise answer remains separate.
- Every published day has exactly one matching `docs/glossary/dayXX.md`; term headings are unique across the glossary directory.
- Every published day has exactly one matching `interview/answers/dayXX.md`; each file contains exactly two stable question IDs and answers every listed follow-up.
- Interview answers state conditions and boundaries rather than absolute implementation claims.
- Every core mechanism contains `实际问题`, `概念落点`, `代码与机制`, and `错误做法与修复`, followed by a short retrieval check.
- A beginner can answer each retrieval check from the example and its state changes without needing assembly, ABI knowledge, or an unstated memory-layout assumption.
- Final diff contains no unrelated files.
