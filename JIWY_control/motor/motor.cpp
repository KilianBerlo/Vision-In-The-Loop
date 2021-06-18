#include "motor.hpp"

Plant::Motor::Motor(uint8_t id, Serial::UART serial) :
    motor_id(id),
    uart(serial)
{ }

void Plant::Motor::setAngle(float angle)
{
    Serial::message tx_msg{};
    tx_msg.motor = motor_id;
    tx_msg.setAngle(angle);
    uart.writeMessage(tx_msg);
}
