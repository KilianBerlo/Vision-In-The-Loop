#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#define TERMINAL    "/dev/ttyS6"

#include <cstring>
#include <unistd.h>
#include <vector>
#include <array>
#include <iostream>

#include <netinet/in.h>
#include "serial_com/Serial.h"

struct tx_message
{
    uint32_t freq : 14;
    uint32_t duty : 14;
    uint32_t direction : 2;
    uint32_t enable : 1;
    uint32_t motor : 1;

    void setDutyCycle (uint8_t duty_cycle)
    {
        duty = uint32_t ((float)freq * ((float)duty_cycle / 100));
    }
};

std::array<uint8_t, 4> convertToArray(tx_message msg)
{
    std::array<uint8_t, 4> temp{};
    std::copy(
            reinterpret_cast<uint8_t *>(&msg),
            reinterpret_cast<uint8_t *>(&msg) + 4,
            &temp[0]
    );
    return temp;
}

/**
 * Motor 0 = tilt
 * Motor 1 = pan
 */

int main()
{
    auto port = Serial(TERMINAL);

    auto tx_msg = tx_message();
    tx_msg.freq = 2500;
    tx_msg.direction = 0b01; // Clockwise (positive)
    tx_msg.enable = 1;
    tx_msg.motor = 0;
    tx_msg.setDutyCycle(63);

    // Enable motor 0.
    port.write_array(convertToArray(tx_msg));

    // Enable motor 1.
    tx_msg.motor = 1;
    port.write_array(convertToArray(tx_msg));

    while(true)
    {
        auto msg = port.readMessage(false);

        if (msg != std::nullopt)
        {
            int32_t count = msg->encoder_value;

            switch(msg->motor)
            {
                case 0 :
                {
                    // Two rotations
                    if (count >= 100)
                    {
                        tx_msg.direction = 0b10; // Counterclockwise (negative count)
                        tx_msg.motor = 0;
                        port.write_array(convertToArray(tx_msg));
                    }

                    if (count < 0)
                    {
                        tx_msg.direction = 0b01; // Clockwise (positive)
                        tx_msg.motor = 0;
                        port.write_array(convertToArray(tx_msg));
                    }

                    std::cout << "Motor 0: "<< count << std::endl;
                    break;
                }
                case 1 :
                {
                    // Two rotations
                    if (count >= 100)
                    {
                        tx_msg.direction = 0b10; // Counterclockwise (negative count)
                        tx_msg.motor = 1;
                        port.write_array(convertToArray(tx_msg));
                    }

                    if (count < 0)
                    {
                        tx_msg.direction = 0b01; // Clockwise (positive)
                        tx_msg.motor = 1;
                        port.write_array(convertToArray(tx_msg));
                    }

                    std::cout << "Motor 1: "<< count << std::endl;
                    break;
                }
            }
        }

        //usleep(10);
    }
}
#pragma clang diagnostic pop