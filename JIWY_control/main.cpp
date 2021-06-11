#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#define TERMINAL    "/dev/ttyS6"

#include <string.h>
#include <unistd.h>
#include <vector>
#include <array>
#include <iostream>
//#include <evdns.h>
#include <netinet/in.h>
#include "serial_com/Serial.h"

struct message
{
    uint32_t freq : 14;
    uint32_t duty : 14;
    uint32_t direction : 2;
    uint32_t enable : 2;

    void setDutyCycle (uint8_t duty_cycle)
    {
        duty = uint32_t ((float)freq * ((float)duty_cycle / 100));
    }
};

std::array<uint8_t, 4> convertToArray(message msg)
{
    std::array<uint8_t, 4> temp{};
    std::copy(
            reinterpret_cast<uint8_t *>(&msg),
            reinterpret_cast<uint8_t *>(&msg) + 4,
            &temp[0]
    );
    return temp;
}

int main()
{
    auto port = Serial(TERMINAL);

    auto msg = message();
    msg.freq = 2500;
    msg.direction = 0b01; // Clockwise (positive)
    msg.enable = 0b10;
    msg.setDutyCycle(60);

    port.write_array(convertToArray(msg));

    while(true)
    {
        auto copy = port.read_array(4);

        if (!copy.empty())
        {
            int32_t count = copy[0] << 24 | copy[1] << 16 | copy[2] << 8 | copy[3];

            // Two rotations
            if (count >= 100)
            {
                msg.direction = 0b10; // Counterclockwise (negative count)
                port.write_array(convertToArray(msg));
            }

            if (count < 0)
            {
                msg.direction = 0b01; // Clockwise (positive)
                port.write_array(convertToArray(msg));
            }

            std::cout << count << std::endl;
        }

    }
}
#pragma clang diagnostic pop