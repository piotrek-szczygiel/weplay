# Tetris for Raspberry Pi

## Building

```bash
Create build directory
$ mkdir build && cd build

Desktop
$ cmake .. -DPLATFORM="Desktop"

Raspberry Pi
$ cmake .. -DPLATFORM="Raspberry Pi"

Build
$ make

Run
$ cd ..
$ build/rpi-tetris
```
