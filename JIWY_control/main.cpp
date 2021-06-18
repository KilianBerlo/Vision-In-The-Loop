#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#define TERMINAL    "/dev/ttyS6"

#include <unistd.h>
#include <vector>
#include <iostream>
#include <csignal>

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
    std::cout << "X_axis: " << x_axis << " Y_axis: " << y_axis << std::endl;
}

/**
 * Motor 0 = tilt
 * Motor 1 = pan
 */
int main()
{
    ImageRecognition opencv = ImageRecognition();
    ImageRecognition::initialize(move_motor);

    std::cout << "Motor driver started." << std::endl;

    tilt_motor.setAngle(1);
    pan_motor.setAngle(5);

    while(true)
    {
        auto msg = serial_port.readMessage(false);

        if (msg != std::nullopt)
        {
            float angle = msg->getAngle();

            //std::cout << angle << std::endl;

            switch(msg->motor)
            {
                // Tilt
                case 0 :
                {
                    std::cout << "Motor 0 angle at: "<< angle << " rad" << std::endl;
                    break;
                }
                // Pan
                case 1 :
                {
                    std::cout << "Motor 1 angle at: "<< angle << " rad" << std::endl;
                    break;
                }
            }
        }
    }
}
#pragma clang diagnostic pop