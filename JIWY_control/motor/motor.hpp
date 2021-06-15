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

        enum class Direction : uint8_t
        {
            STOP_GND =          0b00, // Brake to GND.
            CLOCKWISE =         0b01, // Encoder value increasing.
            COUNTERCLOCKWISE =  0b10, // Encoder value decreasing.
            STOP_VCC =          0b11, // Brake to VCC.
        };

        Motor(uint8_t id, uint32_t frequency, uint32_t steps_per_revolution, Serial::UART uart);

        void setFrequency(uint32_t frequency);

        // Set duty cycle as percentage from 0 - 100
        void setDutyCycle(uint8_t duty_cycle);

        void setDirection(Direction direction);

        void enable();

        void disable();

        void goToInitialPosition(uint8_t duty_cycle);

        double getAngle(int32_t encoder_value);

        double current_position = 0.0;

    private:
        // ID of this motor.
        const uint32_t id;

        const double radians_per_step;

        int32_t encoder_start_position = 0;

        // Instance of the UART.
        Serial::UART uart;

        // Struct which contains the current motor config.
        //Serial::message tx_msg{};
    };


}
#endif //JIWY_CONTROL_MOTOR_HPP
