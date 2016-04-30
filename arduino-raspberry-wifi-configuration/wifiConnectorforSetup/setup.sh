#!/bin/sh
echo "[setup] need root permission"
echo "before check status your wireless lan card is connected"
lsusb
ping 127.0.0.1 -c 3 > /dev/null

cp -rf ./interfaces /etc/network/
echo "[ROOT] copied to /etc/network for Network Configuration (WLAN0)"
