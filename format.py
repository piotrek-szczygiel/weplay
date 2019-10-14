#!/usr/bin/env python3
import os
import pathlib
import subprocess


def collect_files(roots, extensions):
    result = []
    for root in roots:
        for (directory, _, files) in os.walk(root):
            for f in files:
                path = os.path.join(directory, f)
                if pathlib.Path(path).suffix in extensions:
                    result.append(path)

    return result


if __name__ == "__main__":
    extensions = [".c", ".cc", ".cpp", ".h", ".hh", ".hpp"]

    format_base = ["clang-format", "-i"]
    tidy_base = ["clang-tidy", "-extra-arg=-std=c++17", "", "--", "-Iraylib"]

    for f in collect_files(["console", "controller"], extensions):
        command = format_base + [f]
        print("Formatting", f)
        subprocess.run(command)

    for f in collect_files(["console"], extensions):
        command = tidy_base
        command[2] = f
        print("\n\n{1}\nTidying {0}\n{1}".format(f, "#" * (len(f) + 8)))
        result = subprocess.run(command)
