#ifndef JIWY_CONTROL_MOTOR_HPP
#define JIWY_CONTROL_MOTOR_HPP

#include <cstdint>
#include <cmath>

#include "../serial/uart.hpp"
#include "../timer/timer.hpp"

namespace Plant
{
    class Motor
    {
    public:

        Motor(uint8_t motor_id, Serial::UART uart);

        void setAngle(float angle);

    private:

        // Instance of the UART.
        Serial::UART uart;
        uint8_t motor_id;

        // Struct which contains the current motor config.
        //Serial::message tx_msg{};
    };


}
#endif //JIWY_CONTROL_MOTOR_HPP
