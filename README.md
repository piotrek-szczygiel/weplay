# Raspberry Pi Console

## Building
- Install [Boost](https://www.boost.org) libraries
- Build using [CMake](https://cmake.org)

## Windows
- Download and extract [Boost](https://www.boost.org)
- Open terminal in extracted directory

```
bootstrap
.\b2 install
```

- Set `BOOST_ROOT` environment variable to C:\Boost
- From Visual Studio issue "Open a local folder" and point to this repository

## Raspberry Pi

- Install [DietPi](https://dietpi.com) distribution

```bash
# Install some dependencies
raspberry-console$ sudo ./dietpi.sh

# Manually build my fork of raylib
$ git clone https://github.com/piotrek-szczygiel/raylib
raylib/src$ make -j4 PLATFORM=PLATFORM_RPI RAYLIB_LIBTYPE=SHARED
raylib/src$ sudo make install RAYLIB_LIBTYPE=SHARED

# Build this project in separate directory
raspberry-console$ mkdir out && cd out
raspberry-console/out$ cmake .. && make -j4
```

- Project should now compile using globally installed raylib

## ESP8266 controller
- Install [PlatformIO IDE](https://platformio.org/platformio-ide)
- Open `controller` directory
- Build and upload using PlatformIO
