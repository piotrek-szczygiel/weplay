#!/usr/bin/env bash
set -e

apt update
apt install libraspberrypi-dev raspberrypi-kernel-headers build-essential
usermod -aG video dietpi
