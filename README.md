# Raspberry Pi Console

## Building on Raspberry Pi
```bash
Install DietPi dependencies
$ sudo ./dietpi.sh

Create build directory
$ mkdir build && cd build

Compile this project
$ cmake .. -DPLATFORM=PLATFORM_RPI
$ make

Run the console
$ cd ..
$ ./build/raspberry-console
```

## Building on Windows
Open project directory with Visual Studio and everything should work out of the box.

## Building on other platforms
- Follow raylib installation guide on https://github.com/raysan5/raylib#build-and-installation
- Make sure that raylib is installed globally and visible for compilers
- Build project using CMake
