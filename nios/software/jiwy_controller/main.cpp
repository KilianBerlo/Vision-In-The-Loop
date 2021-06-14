#include <stdio.h>
#include <unistd.h>
#include <io.h>
#include <cstring>
#include <algorithm>
#include "system.h"

#include "motor_driver/encoder.hpp"
#include "motor_driver/motor.hpp"
#include "serial/uart.hpp"

// Function prototype.
void handleUartMessage(Serial::rx_message &received_message);

// Setup UART.
Serial::UART uart = Serial::UART(handleUartMessage);

// Setup Encoders.
Plant::Encoder tilt_encoder = Plant::Encoder(QUADRATURE_ENCODER_0_BASE);
Plant::Encoder pan_encoder = Plant::Encoder(QUADRATURE_ENCODER_1_BASE);

// Setup Motors.
Plant::Motor tilt_motor = Plant::Motor(MOTOR_PWM_0_SLAVE_1_BASE, MOTOR_PWM_0_SLAVE_2_BASE);
Plant::Motor pan_motor = Plant::Motor(MOTOR_PWM_1_SLAVE_1_BASE, MOTOR_PWM_1_SLAVE_2_BASE);

void handleUartMessage(Serial::rx_message &received_message)
{
	// Determine for which motor this message is intended.
	switch (received_message.motor)
	{
		case 0:
		{
			tilt_motor.setFrequency(received_message.getFirstWord());
			tilt_motor.setSecondWord(received_message.getSecondWord());
			break;
		}

		case 1:
		{
			pan_motor.setFrequency(received_message.getFirstWord());
			pan_motor.setSecondWord(received_message.getSecondWord());
			break;
		}
	}
}

/**
 * Motor 0 = tilt
 * Motor 1 = pan
 */
int main()
{
	// Print startup message.
	printf("JIWY motor controller running..\n");

	int32_t prev_tilt = 0;
	int32_t prev_pan = 0;

	while(1)
	{
		int32_t tilt = tilt_encoder.readData();

		if (tilt != prev_tilt)
		{
			prev_tilt = tilt;
			Serial::tx_message message(tilt, 0);
			uart.sendData(message);
		}

		int32_t pan = pan_encoder.readData();

		if (pan != prev_pan)
		{
			prev_pan = pan;
			Serial::tx_message message(pan, 1);
			uart.sendData(message);
		}

		//usleep(10);
	}

	return 0;
}
