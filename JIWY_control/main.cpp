#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#define TERMINAL    "/dev/ttyS6"

#include <cstring>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <csignal>

#include "serial/uart.hpp"
#include "motor/motor.hpp"

// SIGINT handler.
void sigIntHandler(int exit_code)
{
    // Stop all motors.
    /*for (auto motor : motors)
    {
        motor.disable();
    }*/

    std::cout << "Stopping motor driver with exit code: " << exit_code << std::endl;
    exit(exit_code);
}

/**
 * Motor 0 = tilt
 * Motor 1 = pan
 */
int main()
{
    // Add a program interruption handler (on ^C).
    signal(SIGINT, sigIntHandler);

    // Establish a serial connection.
    Serial::UART serial_port = Serial::UART(TERMINAL);

    // Create instances of the motors.
    //Plant::Motor tilt_motor = Plant::Motor(0, 2500, 160, serial_port);
    //Plant::Motor pan_motor = Plant::Motor(1, 2500, 80, serial_port);

    // Add to the vector of motors so they can all be safely stopped in case of a SIGINT.
    //motors.push_back(tilt_motor);
    //motors.push_back(pan_motor);

    // Initialize the submodel itself and calculate the outputs for t = 0.0.
    //tilt_model.Initialize(u_tilt, y_tilt, 0.0);

    //Timer clock;
    //clock.tick();

    std::cout << "Motor driver started." << std::endl;

    Serial::message tx{};
    tx.motor = 1;
    tx.setAngle(M_PI * 2);

    serial_port.writeMessage(tx);

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

        usleep(500000);

        float temp = tx.getAngle();
        tx.setAngle( temp + 1);
        serial_port.writeMessage(tx);
    }
}
#pragma clang diagnostic pop