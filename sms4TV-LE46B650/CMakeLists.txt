cmake_minimum_required(VERSION 2.5)
project(sms4tv)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
include(../build/toolchain.cmake)

add_executable(sms4tv
        ${SOURCE_DIR}/main.c)