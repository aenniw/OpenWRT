cmake_minimum_required(VERSION 2.5)
project(remot-scripts)

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
include(../toolchain.cmake)

add_executable(rs-client
        ${SOURCE_DIR}/client.c)

add_executable(rs-server
        ${SOURCE_DIR}/server.c)