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

#include "_20_sim/tilt/PositionControllerTilt.h"
#include "_20_sim/pan/PositionControllerPan.h"

// Vector which holds an x amount of motors.
std::vector<Plant::Motor> motors;

// SIGINT handler.
void sigIntHandler(int exit_code)
{
    // Stop all motors.
    for (auto motor : motors)
    {
        motor.disable();
    }

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
    Plant::Motor tilt_motor = Plant::Motor(0, 2500, 320, serial_port);
    //Plant::Motor pan_motor = Plant::Motor(1, 2500, 80, serial_port);

    // Add to the vector of motors so they can all be safely stopped in case of a SIGINT.
    motors.push_back(tilt_motor);
    //motors.push_back(pan_motor);

    // Initialize the 20-sim models.
    XXDouble u_tilt [2 + 1];
    XXDouble y_tilt [2 + 1];

    // Initialize the input with correct initial values.
    // TODO: add homing and determine the angle.
    u_tilt[0] = 0.0;		/* in */
    u_tilt[1] = 0.0;		/* position */

    y_tilt[0] = 0.0;		/* corr */
    y_tilt[1] = 0.0;		/* out */

    PositionControllerTilt tilt_model;
    PositionControllerTilt pan_model;

    // Initialize the submodel itself and calculate the outputs for t = 0.0.
    tilt_model.Initialize(u_tilt, y_tilt, 0.0);
    std::cout << "Model time is: " << tilt_model.GetTime() << std::endl;

    std::cout << "Motor driver started." << std::endl;

    tilt_motor.setDutyCycle(63);
    tilt_motor.setDirection(Plant::Motor::Direction::CLOCKWISE);
    //tilt_motor.enable();

    //pan_motor.setDutyCycle(63);
    //pan_motor.setDirection(Plant::Motor::Direction::CLOCKWISE);
    //pan_motor.enable();

    while(true)
    {
        auto msg = serial_port.readMessage(false);

        if (msg != std::nullopt)
        {
            int32_t count = msg->encoder_value;

            switch(msg->motor)
            {
                // Tilt
                case 0 :
                {
                    // Two rotations
                    if (count >= 100)
                    {
                        tilt_motor.setDirection(Plant::Motor::Direction::COUNTERCLOCKWISE);
                    }

                    if (count < 0)
                    {
                        tilt_motor.setDirection(Plant::Motor::Direction::CLOCKWISE);
                    }

                    std::cout << "Motor 0: "<< count << std::endl;
                    break;
                }
                // Pan
                case 1 :
                {
                    /* Two rotations
                    if (count >= 100)
                    {
                        pan_motor.setDirection(Plant::Motor::Direction::COUNTERCLOCKWISE);
                    }

                    if (count < 0)
                    {
                        pan_motor.setDirection(Plant::Motor::Direction::COUNTERCLOCKWISE);
                    }*/

                    std::cout << "Motor 1: "<< count << std::endl;
                    break;
                }
            }
        }

        //usleep(10);
    }
}
#pragma clang diagnostic pop