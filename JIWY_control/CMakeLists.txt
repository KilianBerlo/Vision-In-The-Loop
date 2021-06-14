cmake_minimum_required(VERSION 3.16)
project(JIWY_control)

set(CMAKE_CXX_STANDARD 20)

add_executable(JIWY_control
        main.cpp
        serial/uart.cpp
        motor/motor.cpp

        _20_sim/tilt/PositionControllerTilt.cpp
        _20_sim/tilt/common/EulerAngles.cpp
        _20_sim/tilt/common/MotionProfiles.cpp
        _20_sim/tilt/common/xxfuncs.cpp
        _20_sim/tilt/common/xxinteg.cpp
        _20_sim/tilt/common/xxinverse.cpp
        _20_sim/tilt/common/xxmatrix.cpp)