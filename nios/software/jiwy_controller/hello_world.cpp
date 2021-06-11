#include <stdio.h>
#include <io.h>
#include <cstring>
#include <algorithm>
#include "system.h"

#include "serial/uart.hpp"

uint16_t frequency = 0;

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
	if (frequency != received_message.freq)
	{
		IOWR_32DIRECT(MOTOR_PWM_0_SLAVE_1_BASE, 0x00, received_message.freq);
		frequency = received_message.freq;
	}

	uint32_t combined = received_message.getSecondWord();

	IOWR_32DIRECT(MOTOR_PWM_0_SLAVE_2_BASE, 0x00, combined);

	//printf("rx data: %.4s\n", rx_data);
}

int main()
{
	// Print startup message.
	printf("Starting JIWY motor controller..\n");

	// Setup UART.
	Serial::UART uart = Serial::UART(handleUartMessage);

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
