#!/bin/bash
set -e

if [[ $EUID > 0 ]]; then
    echo "Please run as root/sudo"
    exit 1
fi

apt update
apt install libraspberrypi-dev libboost-all-dev raspberrypi-kernel-headers build-essential cmake hostapd dnsmasq iptables

usermod -aG video dietpi

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

systemctl start create_ap
systemctl enable create_ap
