cmake_minimum_required(VERSION 3.5)
project(OpenWRT)

execute_process(COMMAND ${CMAKE_HOME_DIRECTORY}/configure ${CMAKE_HOME_DIRECTORY})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -std=c11 -Wall")

add_subdirectory(remote-scripts)
add_subdirectory(sms4TV-LE46B650)
add_subdirectory(usbip-overlay)
