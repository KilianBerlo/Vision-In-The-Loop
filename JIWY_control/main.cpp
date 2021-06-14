#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#define TERMINAL    "/dev/ttyS6"

#include <cstring>
#include <unistd.h>
#include <vector>
#include <iostream>
#include <csignal>
#include <time.h>
#include <chrono>

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
    Plant::Motor tilt_motor = Plant::Motor(0, 2500, 160, serial_port);
    //Plant::Motor pan_motor = Plant::Motor(1, 2500, 80, serial_port);

    // Add to the vector of motors so they can all be safely stopped in case of a SIGINT.
    motors.push_back(tilt_motor);
    //motors.push_back(pan_motor);

    // Motor homing to go to the initial positions.
    tilt_motor.goToInitialPosition(60);

    // Initialize the 20-sim models.
    XXDouble u_tilt [3 + 1];
    XXDouble y_tilt [1 + 1];

    // Initialize the input with correct initial values.
    // TODO: add homing and determine the angle.
    u_tilt[0] = 0.0;		/* corr */
    u_tilt[1] = 12.0;		/* in */
    u_tilt[2] = 0.0;		/* position */

    y_tilt[0] = 0.0;		/* out */

    PositionControllerTilt tilt_model;
    PositionControllerTilt pan_model;

    // Initialize the submodel itself and calculate the outputs for t = 0.0.
    tilt_model.Initialize(u_tilt, y_tilt, 0.0);

    Timer clock;
    clock.tick();

    std::cout << "Motor driver started." << std::endl;

    //u_tilt[1] = 1.0;

    while(true)
    {
        auto msg = serial_port.readMessage(false);

        clock.tock();

        // Every 10ms.
        if (clock.duration().count() >= 10)
        {
            // Time step!
            clock.tick();

            u_tilt[2] = tilt_motor.current_position;

            tilt_model.Calculate(u_tilt, y_tilt);
            int32_t percentage = int32_t (y_tilt[0] * 100);

            if (percentage > 0)
            {
                tilt_motor.setDirection(Plant::Motor::Direction::CLOCKWISE);
            }
            else if (percentage < 0)
            {
                tilt_motor.setDirection(Plant::Motor::Direction::COUNTERCLOCKWISE);
            }

            tilt_motor.setDutyCycle(percentage);
            tilt_motor.enable();

            std::cout << "Duty cycle: " << percentage << " Encoder: " << tilt_motor.current_position <<  " time: " << tilt_model.GetTime() << std::endl;
        }


        if (msg != std::nullopt)
        {
            int32_t count = msg->encoder_value;

            switch(msg->motor)
            {
                // Tilt
                case 0 :
                {
                    double angle = tilt_motor.getAngle(count);
                    tilt_motor.current_position = angle;
                    //std::cout << "Motor 0 angle at: "<< angle << " rad" << std::endl;
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

                    //std::cout << "Motor 1: "<< count << std::endl;
                    break;
                }
            }
        }

        //usleep(10);
    }
}
#pragma clang diagnostic pop