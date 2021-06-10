#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#define TERMINAL    "/dev/ttyUSB0"

#include <string.h>
#include <unistd.h>
#include <vector>
#include <array>
#include <iostream>
#include <evdns.h>
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

    uint32_t getFullField()
    {
        return (freq << 18) | (duty << 4) | (direction << 2) | enable;
    }

    std::array<uint8_t,4> getAsArray()
    {
        std::array<uint8_t ,4 > array;
        (uint32_t&)*array.data() = htonl(getFullField());
        return array;
    }
};

int main()
{
    auto port = Serial(TERMINAL);

    auto msg = message();
    msg.freq = 2500;
    msg.direction = 0b01; // Clockwise (positive)
    msg.enable = 0b10;
    msg.setDutyCycle(2);

    port.write_array(msg.getAsArray());

    while(true)
    {
        //std::cout << "sending bytes..." << std::endl;



        //sleep(2);
        //msg.direction = 0b10;

        /*for (int i = 40; i < 100; i++)
        {
            msg.setDutyCycle(i);

            usleep(100000);
        }*/

        //port.write_array(msg.getAsArray());
        //sleep(2);
        //msg.direction = 0b01;




        //uint32_t full_data = msg.getFullField();


        auto copy = port.read_array(4);

        if (!copy.empty())
        {
            int32_t count = copy[0] << 24 | copy[1] << 16 | copy[2] << 8 | copy[3];

            // Two rotations
            if (count >= 100)
            {
                msg.direction = 0b10; // Counterclockwise (negative count)
                port.write_array(msg.getAsArray());
            }

            if (count < 0)
            {
                msg.direction = 0b01; // Clockwise (positive)
                port.write_array(msg.getAsArray());
            }

            std::cout << count << std::endl;

            // loop through the array elements
            /*for (auto i: copy)
            {
                std::cout << +i << ' ';
            }
            std::cout << std::endl;*/
        }

    }
}
#pragma clang diagnostic pop