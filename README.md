# Raspberry Pi Console

## Building
- Build using [CMake](https://cmake.org)

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

