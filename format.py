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
    roots = ["src", "controller"]
    extensions = [".cc", ".cpp", ".h", ".hpp", ".ino"]
    files = collect_files(roots, extensions)

    format_base = ["clang-format", "-i"]
    tidy_base = ["clang-tidy", "", "--", "-Iraylib"]

    for f in files:
        command = format_base + [f]
        print("Formatting", f)
        subprocess.run(command)

    for f in files:
        command = tidy_base
        command[1] = f
        print("\n\n{1}\nTidying {0}\n{1}".format(f, "#" * (len(f) + 8)))
        result = subprocess.run(command)
