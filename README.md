# Tetris for Raspberry Pi

## Dependencies
- [raylib](https://github.com/raysan5/raylib)

## Building
```bash
Create build directory
$ mkdir build && cd build

Desktop
$ cmake ..

Raspberry Pi
$ cmake .. -DPLATFORM=RPI

Build
$ make

Run
$ cd ..
$ build/rpi-tetris
```
