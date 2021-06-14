#include "motor.hpp"

Plant::Motor::Motor(uint8_t id, uint32_t frequency, Serial::UART uart) :
    id(id),
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
