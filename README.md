# [OpenWRT](https://openwrt.org/)

[![N|Solid](https://travis-ci.org/aenniw/OpenWRT.svg?branch=master)](https://travis-ci.org/aenniw/OpenWRT)

Applications/ports for [TL-WR1043ND](https://wiki.openwrt.org/toh/tp-link/tl-wr1043nd) and other stuff for managing it.

[**Toolchains and binaries**](https://downloads.openwrt.org/) /
[**Wiki**](https://wiki.openwrt.org/) / 
[**Forum**](https://forum.openwrt.org/)

## Setup environment
```
sudo apt-get update
sudo apt-get install cmake make
```

## Build
```
cmake ./CMakeLists.txt
for PROJECT in $(ls -d ./*/CMakeLists.txt); do \
    cd ${PROJECT%*/*} && ls; cd ..; \
done
```

## SubProjects
[Usbip-overlay](usbip-overlay/README.md) |
[Sms4TV](sms4TV-LE46B650/README.md) |
[Remote-Scripts](remote-scripts/README.md)