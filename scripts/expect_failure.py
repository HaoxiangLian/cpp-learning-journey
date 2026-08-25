#!/usr/bin/env python3
"""Verify that a labelled teaching example fails in the intended build stage."""

from __future__ import annotations

import argparse
import pathlib
import subprocess
import tempfile


def run(command: list[str]) -> subprocess.CompletedProcess[str]:
    return subprocess.run(command, text=True, capture_output=True, check=False)


def main() -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--compiler", required=True)
    parser.add_argument("--stage", choices=("compile", "link"), required=True)
    parser.add_argument("--source", type=pathlib.Path, required=True)
    args = parser.parse_args()

    common = [args.compiler, "-std=c++17", "-Wall", "-Wextra", "-Wpedantic"]

    with tempfile.TemporaryDirectory(prefix="cpp-learning-failure-") as temp_dir:
        temp = pathlib.Path(temp_dir)
        object_file = temp / "exercise.o"
        compile_result = run(common + ["-c", str(args.source), "-o", str(object_file)])

        if args.stage == "compile":
            if compile_result.returncode == 0:
                print(f"expected compilation to fail: {args.source}")
                return 1
            return 0

        if compile_result.returncode != 0:
            print("expected source to compile before the intentional link failure")
            print(compile_result.stderr)
            return 1

        executable = temp / "exercise"
        link_result = run([args.compiler, str(object_file), "-o", str(executable)])
        if link_result.returncode == 0:
            print(f"expected linking to fail: {args.source}")
            return 1

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
