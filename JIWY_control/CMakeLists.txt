cmake_minimum_required(VERSION 3.16)
project(JIWY_control)

set(CMAKE_CXX_STANDARD 20)

add_executable(JIWY_control main.cpp serial_com/Serial.cpp serial_com/Serial.h)