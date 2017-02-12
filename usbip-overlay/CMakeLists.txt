cmake_minimum_required(VERSION 3.5)
project(usbipd-overlay)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
include(../toolchain.cmake)

add_executable(usbipd-overlay
        ${SOURCE_DIR}/main.c
        ${SOURCE_DIR}/common.c
        ${SOURCE_DIR}/base64.c)