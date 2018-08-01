# [OpenWRT](https://openwrt.org/)

[![N|Solid](https://travis-ci.org/aenniw/OpenWRT.svg?branch=master)](https://travis-ci.org/aenniw/OpenWRT)

Applications/ports for [TL-WR1043ND](https://wiki.openwrt.org/toh/tp-link/tl-wr1043nd)
[ASUS-AC58U](https://forum.lede-project.org/t/asus-ac58u-what-about/1691) and other stuff for managing it.

[**Toolchains and binaries**](https://downloads.openwrt.org/) /
[**Wiki**](https://wiki.openwrt.org/) /
[**Forum**](https://forum.openwrt.org/) /
[**ASUS-AC58U zyxmon**](http://lede-ac58u.zyxmon.org/) or [**ASUS-AC58U OpenWrt**](https://openwrt.org/toh/hwdata/asus/asus_rt-ac58u)

## Setup environment

```bash
sudo apt-get update
sudo apt-get install cmake make wget tar
```

## Build

To change toolchain add architecture to CMake like so -Dar71xx=true or -Dipq40xx=true

```bash
./make ar70xx
./make ipq40xx
```

## SubProjects

[Usbip-overlay](usbip-overlay/README.md) |
[Sms4TV](sms4TV-LE46B650/README.md) |
[Remote-Scripts](remote-scripts/README.md)
