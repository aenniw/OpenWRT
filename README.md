# [OpenWRT](https://openwrt.org/)

[![N|Solid](https://travis-ci.org/aenniw/OpenWRT.svg?branch=master)](https://travis-ci.org/aenniw/OpenWRT)

Applications/ports for [TL-WR1043ND](https://wiki.openwrt.org/toh/tp-link/tl-wr1043nd)
[ASUS-AC58U](https://forum.lede-project.org/t/asus-ac58u-what-about/1691) and other stuff for managing it.

[**Toolchains and binaries**](https://downloads.openwrt.org/) /
[**Wiki**](https://wiki.openwrt.org/) / 
[**Forum**](https://forum.openwrt.org/) /
[**ASUS-AC58U flashing**](http://lede-ac58u.zyxmon.org/)

## Setup environment
```
sudo apt-get update
sudo apt-get install cmake make wget tar
```

## Build
```
for ARCH in ipq806x ar71xx; do
    cmake ./CMakeLists.txt -D${ARCH}=true;
    for PROJECT in $(ls -d ./*/CMakeLists.txt); do
        cd ${PROJECT%*/*} && \
          make && \
          make clean && \
          cd ..;
    done;
done
```

## SubProjects
[Usbip-overlay](usbip-overlay/README.md) |
[Sms4TV](sms4TV-LE46B650/README.md) |
[Remote-Scripts](remote-scripts/README.md)
