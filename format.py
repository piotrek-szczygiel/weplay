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


def tidy(file):
    tidy_base = [
        "clang-tidy",
        "-extra-arg=-std=c++17",
        "",
        "--",
        "-Ilibraries/raylib",
        "-Ilibraries/asio",
        "-Ilibraries/easyloggingpp",
        "-DASIO_STANDALONE",
        "-D_WIN32_WINNT=0x0A00",
    ]

    command = tidy_base
    command[2] = f
    print("\n\n{1}\nTidying {0}\n{1}".format(f, "#" * (len(f) + 8)))
    subprocess.run(command)


if __name__ == "__main__":
    extensions = [".c", ".cc", ".cpp", ".h", ".hh", ".hpp"]

    if len(sys.argv) > 1:
        for f in sys.argv[1:]:
            format(f)

        for f in sys.argv[1:]:
            tidy(f)
    else:
        for f in collect_files(["console", "controller"], extensions):
            format(f)

        for f in collect_files(["console"], extensions):
            tidy(f)
