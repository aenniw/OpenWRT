cmake_minimum_required(VERSION 2.5)
project(OpenWRT)

if (ipq806x)
    execute_process(COMMAND ${CMAKE_HOME_DIRECTORY}/configure ${CMAKE_HOME_DIRECTORY}
            https://archive.openwrt.org/snapshots/trunk/ipq806x/generic/OpenWrt-SDK-ipq806x_gcc-5.3.0_musl-1.1.16_eabi.Linux-x86_64.tar.bz2)
else ()
    execute_process(COMMAND ${CMAKE_HOME_DIRECTORY}/configure ${CMAKE_HOME_DIRECTORY}
            https://archive.openwrt.org/snapshots/trunk/ar71xx/generic/OpenWrt-SDK-ar71xx-generic_gcc-5.3.0_musl-1.1.16.Linux-x86_64.tar.bz2)
endif ()
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -std=c11 -Wall")

add_subdirectory(remote-scripts)
add_subdirectory(sms4TV-LE46B650)
add_subdirectory(usbip-overlay)
