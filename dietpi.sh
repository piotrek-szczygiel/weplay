#!/usr/bin/env bash
set -e

if [[ $EUID -gt 0 ]]; then
    echo "Please run as root/sudo"
    exit 1
fi

if [[ "$(uname -n)" != "DietPi" ]]; then
    echo "You are running $(uname -n) instead of the DietPi distribution"
    exit 1
fi

echo "Updating the system"
apt update -y
apt full-upgrade -y

echo "Installing dependencies"
apt install -y libraspberrypi-dev raspberrypi-kernel-headers \
               build-essential cmake hostapd dnsmasq iptables

usermod -aG video dietpi

echo "Installing create_ap"
pushd /tmp
git clone https://github.com/oblique/create_ap
pushd create_ap
make install
popd
popd

echo "Configuring Access Point"
cat << EOF > /etc/create_ap.conf
CHANNEL=default
GATEWAY=192.168.12.1
WPA_VERSION=1+2
ETC_HOSTS=0
DHCP_DNS=gateway
NO_DNS=0
NO_DNSMASQ=0
HIDDEN=0
MAC_FILTER=0
MAC_FILTER_ACCEPT=/etc/hostapd/hostapd.accept
ISOLATE_CLIENTS=0
SHARE_METHOD=none
IEEE80211N=0
IEEE80211AC=0
HT_CAPAB=[HT40+]
VHT_CAPAB=
DRIVER=nl80211
NO_VIRT=0
COUNTRY=
FREQ_BAND=2.4
NEW_MACADDR=
DAEMONIZE=0
NO_HAVEGED=0
WIFI_IFACE=wlan0
INTERNET_IFACE=
SSID=RaspberryConsole
PASSPHRASE=Korobeiniki1984
USE_PSK=0
EOF

systemctl daemon-reload
systemctl start create_ap
systemctl enable create_ap

echo "Installing raylib"
pushd /tmp
wget https://github.com/piotrek-szczygiel/raylib/archive/master.tar.gz
tar -zxf master.tar.gz
pushd raylib-master/src
make PLATFORM=PLATFORM_RPI RAYLIB_LIBTYPE=SHARED -j4
make install RAYLIB_LIBTYPE=SHARED
popd
popd
