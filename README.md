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
# Install dependencies and configurations
$ sudo ./dietpi.sh

# Build this project in separate directory
$ mkdir out && cd out
$ cmake .. -DRASPBERRY_PI=ON && make -j4
```

## ESP8266 controller
- Install [PlatformIO IDE](https://platformio.org/platformio-ide)
- Open `controller` directory
- Build and upload using PlatformIO

