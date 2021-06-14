#include <iostream>
#include <chrono>
#include "motor.hpp"

Plant::Motor::Motor(uint8_t id, uint32_t frequency, uint32_t steps_per_revolution, Serial::UART uart) :
    id(id),
    radians_per_step((360.0 / double(steps_per_revolution)) * (M_PI / 180.0)),
    uart(uart)
{
    // Set motor id.
    tx_msg.motor = id;

    // Set frequency.
    tx_msg.freq = frequency;

    // Set direction without performing a write.
    tx_msg.direction = static_cast<uint32_t>(Direction::STOP_GND);
}

void Plant::Motor::setFrequency(uint32_t frequency)
{
    tx_msg.freq = frequency;
    uart.writeMessage(tx_msg);
}

void Plant::Motor::setDutyCycle(uint8_t duty_cycle)
{
    tx_msg.setDutyCycle(duty_cycle);
    uart.writeMessage(tx_msg);
}

void Plant::Motor::setDirection(Plant::Motor::Direction direction)
{
    tx_msg.direction = static_cast<uint32_t>(direction);
    uart.writeMessage(tx_msg);
}

void Plant::Motor::enable()
{
    tx_msg.enable = 1;
    uart.writeMessage(tx_msg);
}

void Plant::Motor::disable()
{
    tx_msg.enable = 0;
    uart.writeMessage(tx_msg);
}

void Plant::Motor::goToInitialPosition(uint8_t duty_cycle)
{
    setDutyCycle(duty_cycle);
    setDirection(Plant::Motor::Direction::COUNTERCLOCKWISE);
    enable();

    int32_t last_value = 0;

    Timer clock;
    clock.tick();

    while (true)
    {
        auto msg = uart.readMessage(false);

        if (msg != std::nullopt && msg->motor == id)
        {
            // Value changed!
            clock.tick();
            last_value = msg->encoder_value;
        }
        else
        {
            clock.tock();
            if (clock.duration().count() >= 100)
            {
                // Halt the power to the motor.
                disable();
                encoder_start_position = last_value;
                std::cout << "Motor : " << id << " has initial value at: " << encoder_start_position << std::endl;
                break;
            }
        }
    }
}

double Plant::Motor::getAngle(int32_t encoder_value)
{
    return (encoder_value - encoder_start_position) * radians_per_step;
}
