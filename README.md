# Raspberry Pi Console

## Building on Raspberry Pi
```bash
Install DietPi dependencies
$ sudo ./dietpi.sh

Create build directory
$ mkdir build && cd build

Compile raylib
$ make -C../raylib/src PLATFORM=PLATFORM_RPI

Compile this project
$ cmake .. -DPLATFORM=PLATFORM_RPI
$ make

Run the console
$ cd .. && build/console
```

## Building on other platforms

- Follow raylib installation guide on https://github.com/raysan5/raylib#build-and-installation
- Build console using CMake
