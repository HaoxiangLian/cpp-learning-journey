# Repository maintenance rules

These rules apply to every automated or manual course update.

## Course invariants

- Simplified Chinese prose, C++ identifiers and standard terms in English.
- C++17 is the primary standard. Mark C++11/14 origins and C++20 additions explicitly.
- Follow `docs/lecture-writing-guide.md`: do not create a standalone daily term-card section. Introduce a new term inside the relevant core mechanism, repeat the glossary's canonical one-sentence definition, and link its anchor at first use.
- `docs/glossary.md` owns the canonical definition index. If a definition changes, audit the glossary, its first-use lecture, exercises, and interview answers together; do not create a competing paraphrased definition.
- Introduce at most five major concepts per day. Do not use an unexplained technical term to define a new term.
- Separate standard guarantees from common compiler, OS, ABI, stack/heap, vptr, and vtable implementations.
- Never equate undefined behavior with a guaranteed crash or segmentation fault.
- Intentionally invalid code belongs under `exercises/dayXX/broken/`, carries an `INTENTIONALLY INVALID` banner, and is excluded from normal CMake targets.
- Do not modify `.obsidian/` as part of course publication.

## Required daily files

- `days/dayXX.md`
- `examples/dayXX/` with at least one independently runnable C++17 example
- `exercises/dayXX/README.md` without answers
- `solutions/dayXX/README.md` and solution code
- updates, only when needed, to `docs/glossary.md`, `interview/question-bank.md`, `CMakeLists.txt`, `progress.md`, `release/manifest.json`, and `release/state.json`

## Publication protocol

1. Read `release/state.json` from the default branch.
2. If `status` is `completed`, stop without writing.
3. Use exactly `next_day`; if its day file is already present and the state disagrees, stop and report the inconsistency.
4. A scheduled run must compare the manifest's `scheduled_date` for `next_day` with the current date in the state's timezone. If the date is still in the future, stop without writing. An explicit user request to “开始第 X 天学习” is a manual early-release override only when X equals `next_day`.
5. Create one branch named `release/dayXX-YYYY-MM-DD` from the current default-branch head.
6. Generate only that day's material and the allowed indexes/state files.
7. Run the repository validator and GCC/Clang C++17 builds through CI. Do not merge on failure.
8. Merge only after required checks pass. Then append the day to `published_days`, advance `next_day` by one, and record the commit/PR in state.
9. After day 24, set `status` to `completed`, `next_day` to `null`, and never create day 25.

Repeated runs must be idempotent: an existing published day, branch, or PR is resumed or reported, never duplicated.

## Validation checklist

- Correct examples compile with warnings enabled on GCC and Clang.
- Relevant resource/lifetime examples run under AddressSanitizer and UndefinedBehaviorSanitizer.
- Markdown relative links resolve; conflict markers are absent.
- Each daily lecture has every required section and the exercise answer remains separate.
- Interview answers state conditions and boundaries rather than absolute implementation claims.
- Every core mechanism contains `实际问题`, `概念落点`, `代码与机制`, and `错误做法与修复`, followed by a short retrieval check.
- Final diff contains no unrelated files.
