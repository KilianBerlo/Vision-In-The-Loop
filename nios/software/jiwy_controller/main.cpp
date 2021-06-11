#include <stdio.h>
#include <io.h>
#include <cstring>
#include <algorithm>
#include "system.h"

#include "motor_driver/motor.hpp"
#include "serial/uart.hpp"

// Function prototype.
void handleUartMessage(Serial::rx_message &received_message);

// Setup UART.
Serial::UART uart = Serial::UART(handleUartMessage);

// Setup Motors.
Plant::Motor motor_tilt = Plant::Motor(MOTOR_PWM_0_SLAVE_1_BASE, MOTOR_PWM_0_SLAVE_2_BASE);
Plant::Motor motor_pan = Plant::Motor(MOTOR_PWM_1_SLAVE_1_BASE, MOTOR_PWM_1_SLAVE_2_BASE);

int update_encoder_value(Serial::UART uart, uint32_t target, int previous_value)
{
	int encoder_value = IORD_32DIRECT(target, 0x00);
	int encoder_copy = encoder_value;


	if (encoder_value != previous_value)
	{
		//printf("Encoder updated to: %d \n\r", encoder_value);

		// Value has been updated, send it!
		std::array<uint8_t, 4> values;
		values[3] = (uint8_t)encoder_value;
		values[2] = (uint8_t)(encoder_value>>=8);
		values[1] = (uint8_t)(encoder_value>>=8);
		values[0] = (uint8_t)(encoder_value>>=8);

		uart.sendData(values);
	}

	return encoder_copy;
}

void handleUartMessage(Serial::rx_message &received_message)
{
	// Determine for which motor this message is intended.
	switch (received_message.motor)
	{
		case 0:
		{
			motor_tilt.setFrequency(received_message.getFirstWord());
			motor_tilt.setSecondWord(received_message.getSecondWord());
		}

		case 1:
		{
			motor_pan.setFrequency(received_message.getFirstWord());
			motor_pan.setSecondWord(received_message.getSecondWord());
		}
	}
}

int main()
{
	// Print startup message.
	printf("JIWY motor controller running..\n");

	int prev_encoder_tilt = 0;
	int prev_encoder_pan = 0;

	while(1)
	{
		prev_encoder_tilt = update_encoder_value(uart, QUADRATURE_ENCODER_0_BASE, prev_encoder_tilt);
		//prev_encoder_pan = update_encoder_value(QUADRATURE_ENCODER_1_BASE, prev_encoder_pan);
		//usleep(delay);
	}

	return 0;
}
