#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#define TERMINAL "/dev/ttyS6"

#include <unistd.h>

#include "serial/uart.hpp"
#include "motor/motor.hpp"
#include "opencv/image_recognition.hpp"

// Establish a serial connection.
Serial::UART serial_port = Serial::UART(TERMINAL);

// Create instances of the motors.
Plant::Motor tilt_motor = Plant::Motor(0, serial_port);
Plant::Motor pan_motor = Plant::Motor(1, serial_port);

void move_motor(float x_axis, float y_axis)
{
    //std::cout << "X_axis: " << x_axis << " Y_axis: " << y_axis << std::endl;
    tilt_motor.setAngle(y_axis);
    pan_motor.setAngle(x_axis);
}

/**
 * Motor 0 = tilt
 * Motor 1 = pan
 */
int main()
{
    ImageRecognition opencv = ImageRecognition();

    // Initialize and run OpenCV.
    ImageRecognition::initialize(move_motor);
    ImageRecognition::run();

    while(true)
    {
        usleep(100000);
    }
}
#pragma clang diagnostic pop