cmake_minimum_required(VERSION 3.16)
project(JIWY_control)

set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GST REQUIRED IMPORTED_TARGET  gstreamer-1.0)
#pkg_check_modules(GSTAPP REQUIRED IMPORTED_TARGET  gstreamer-app-1.0)

include_directories(
        ${OpenCV_INCLUDE_DIRS}
        /usr/include/gstreamer-1.0
        /usr/include/glib-2.0
        /usr/lib/arm-linux-gnueabihf/glib-2.0/include
        )

add_executable(JIWY_control
        main.cpp
        serial/uart.cpp
        motor/motor.cpp
        opencv/image_recognition.cpp
        )

#linking Gstreamer library with target executable

target_link_libraries(JIWY_control
        PkgConfig::GST
        )

#target_link_libraries(${PROJECT_NAME} ${GSTREAMER_LIBRARIES})
target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} )