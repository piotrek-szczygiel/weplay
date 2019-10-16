#!/usr/bin/env python3
import os
import pathlib
import subprocess
import sys


def collect_files(roots, extensions):
    result = []
    for root in roots:
        for (directory, _, files) in os.walk(root):
            for f in files:
                path = os.path.join(directory, f)
                if pathlib.Path(path).suffix in extensions:
                    result.append(path)

    return result


def format(file):
    format_base = ["clang-format", "-i"]
    command = format_base + [f]
    print("Formatting", f)
    subprocess.run(command)


if __name__ == "__main__":
    extensions = [".c", ".cc", ".cpp", ".h", ".hh", ".hpp"]

    if len(sys.argv) > 1:
        for f in sys.argv[1:]:
            format(f)
    else:
        for f in collect_files(["console", "controller"], extensions):
            format(f)
