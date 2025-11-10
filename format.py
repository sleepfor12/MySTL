#!/usr/bin/env python3

from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path
from typing import Iterable, List


PROJECT_ROOT = Path(__file__).resolve().parent
TARGET_DIRECTORIES = ("include", "tests", "examples")
SUPPORTED_SUFFIXES = (
    ".c",
    ".cc",
    ".h",
    ".hpp"
)


def collect_source_files(root: Path) -> List[Path]:
    """收集需要格式化的源码文件。"""
    collected: List[Path] = []
    for relative_dir in TARGET_DIRECTORIES:
        target_dir = root / relative_dir
        if not target_dir.exists():
            continue
        for path in target_dir.rglob("*"):
            if path.is_file() and path.suffix.lower() in SUPPORTED_SUFFIXES:
                collected.append(path)
    collected.sort()
    return collected


def run_clang_format(
    clang_format: str, files: Iterable[Path], check_only: bool, verbose: bool
) -> int:
    """对指定文件运行 clang-format。"""
    if check_only:
        arguments = [clang_format, "--dry-run", "--Werror"]
    else:
        arguments = [clang_format, "-i"]

    exit_code = 0
    for file_path in files:
        command = [*arguments, str(file_path)]
        if verbose:
            print(" ".join(command))
        try:
            subprocess.run(command, check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        except FileNotFoundError:
            print(f"未找到 clang-format 可执行文件：{clang_format}", file=sys.stderr)
            return 1
        except subprocess.CalledProcessError as error:
            exit_code = error.returncode
            print(error.stderr.decode(errors="replace"), file=sys.stderr)
    return exit_code


def parse_arguments() -> argparse.Namespace:
    """解析命令行参数。"""
    parser = argparse.ArgumentParser(description="使用 clang-format 格式化项目源码。")
    parser.add_argument(
        "--clang-format",
        "-c",
        default="clang-format",
        help="clang-format 可执行文件路径，默认为 PATH 中的 clang-format。",
    )
    parser.add_argument(
        "--check",
        action="store_true",
        help="仅检查格式（使用 clang-format --dry-run --Werror），不直接写回文件。",
    )
    parser.add_argument("--verbose", "-v", action="store_true", help="输出详细执行信息。")
    return parser.parse_args()


def main() -> int:
    args = parse_arguments()
    source_files = collect_source_files(PROJECT_ROOT)
    if not source_files:
        print("未找到需要格式化的文件。")
        return 0
    return run_clang_format(args.clang_format, source_files, args.check, args.verbose)


if __name__ == "__main__":
    sys.exit(main())

