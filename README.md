# Tetris for Raspberry Pi

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
