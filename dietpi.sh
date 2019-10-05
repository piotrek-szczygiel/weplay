#!/usr/bin/env bash
set -e

apt update
apt install libraspberrypi-dev raspberrypi-kernel-headers build-essential cmake
usermod -aG video dietpi
