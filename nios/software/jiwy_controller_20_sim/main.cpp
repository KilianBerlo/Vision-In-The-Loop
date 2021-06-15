#include <stdio.h>
#include <io.h>
//#include <cstring>
//#include <algorithm>
#include "system.h"

#include "motor_driver/motor.hpp"
#include "serial/uart.hpp"
#include "timer/timer.hpp"

#include "20-sim/tilt/PositionControllerTilt.h"
#include "20-sim/pan/PositionControllerPan.h"

#define PWM_FREQUENCY 2500 // 20KHZ

// Function prototypes.
void handleUartEvent(Serial::rx_message &received_message);
void handleTimerEvent();

// Setup UART.
Serial::UART uart = Serial::UART(handleUartEvent);

// Setup Timer.
Timer timer = Timer(handleTimerEvent);

// Setup Motors.
Plant::Motor tilt_motor = Plant::Motor(MOTOR_PWM_0_SLAVE_1_BASE, MOTOR_PWM_0_SLAVE_2_BASE, PWM_FREQUENCY, QUADRATURE_ENCODER_0_BASE, 160);
Plant::Motor pan_motor = Plant::Motor(MOTOR_PWM_1_SLAVE_1_BASE, MOTOR_PWM_1_SLAVE_2_BASE, PWM_FREQUENCY, QUADRATURE_ENCODER_1_BASE, 160);

// Motor input and output value arrays.
XXDouble u_tilt [3 + 1];
XXDouble y_tilt [1 + 1];

XXDouble u_pan [3 + 1];
XXDouble y_pan [1 + 1];

// Definitions of 20-sim models.
PositionControllerTilt tilt_model;
PositionControllerPan pan_model;

volatile bool tilt_homing_done = false;
volatile bool pan_homing_done = false;

void handleTimerEvent()
{
	if(tilt_homing_done && pan_homing_done)
	{
		u_tilt[2] = tilt_motor.getAngle();
		u_pan[2] = pan_motor.getAngle();

		if (tilt_model.state != PositionControllerPan::finished)
		{
			tilt_model.Calculate(u_tilt, y_tilt);
			tilt_motor.setSpeedAndDirection(int8_t (y_tilt[0] * 100));
		}
		else
		{
			tilt_motor.disable();
		}

		if (pan_model.state != PositionControllerPan::finished)
		{
			pan_model.Calculate(u_pan, y_pan);
			pan_motor.setSpeedAndDirection(int8_t (y_pan[0] * 100));
		}
		else
		{
			pan_motor.disable();
		}
	}
	else
	{
		if(tilt_motor.isAtInitialPosition())
		{
			tilt_homing_done = true;
			tilt_motor.disable();
		}
	}
}

void handleUartEvent(Serial::rx_message &received_message)
{
	// Determine for which motor this message is intended.
	switch (received_message.motor)
	{
		case 0:
		{
			//tilt_motor.setFrequency(received_message.getFirstWord());
			//tilt_motor.setSecondWord(received_message.getSecondWord());
			break;
		}

		case 1:
		{
			//pan_motor.setFrequency(received_message.getFirstWord());
			//pan_motor.setSecondWord(received_message.getSecondWord());
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

	u_pan[0] = 0.0;		/* corr */
	u_pan[1] = 0.0;		/* in */
	u_pan[2] = 0.0;		/* position */

	u_tilt[0] = 0.0;		/* corr */
	u_tilt[1] = 6.0;		/* in */
	u_tilt[2] = 0.0;		/* position */

	//pan_model.Initialize(u_pan, y_pan, 0.0);
	tilt_model.Initialize(u_tilt, y_tilt, 0.0);

	// Let the motor run to the origin position.
	//pan_motor.setSpeedAndDirection(-63);
	tilt_motor.setSpeedAndDirection(-63);

	timer.enable();

	//int32_t prev_tilt = 0;
	//int32_t prev_pan = 0;

	while(1)
	{



		/*int32_t tilt = tilt_encoder.readData();

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
		}*/

	}

	return 0;
}
