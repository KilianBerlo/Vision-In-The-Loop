#include <stdio.h>
#include <io.h>
#include <unistd.h>
#include "system.h"

#include "motor_driver/motor.hpp"
#include "serial/uart.hpp"
#include "timer/timer.hpp"

#include "20-sim/tilt/PositionControllerTilt.h"
#include "20-sim/pan/PositionControllerPan.h"

#define PWM_FREQUENCY 2500 // 20KHZ

// Function prototypes.
void handleUartEvent(Serial::message &received_message);
void handleTimerEvent();

// Setup UART.
Serial::UART uart = Serial::UART(handleUartEvent);

// Setup Timer.
Timer timer = Timer(handleTimerEvent);

// Setup Motors.
Plant::Motor tilt_motor = Plant::Motor(MOTOR_PWM_0_SLAVE_1_BASE, MOTOR_PWM_0_SLAVE_2_BASE, PWM_FREQUENCY, QUADRATURE_ENCODER_0_BASE, 80);
Plant::Motor pan_motor = Plant::Motor(MOTOR_PWM_1_SLAVE_1_BASE, MOTOR_PWM_1_SLAVE_2_BASE, PWM_FREQUENCY, QUADRATURE_ENCODER_1_BASE, 80);

// Motor input and output value arrays.
XXDouble u_tilt [3 + 1];
XXDouble y_tilt [1 + 1];

//XXDouble u_pan [2 + 1];
//XXDouble y_pan [2 + 1];
XXDouble u_pan [3 + 1];
XXDouble y_pan [1 + 1];

// Definitions of 20-sim models.
PositionControllerTilt tilt_model;
//PositionControllerPan pan_model;
PositionControllerTilt pan_model;

volatile bool tilt_homing_done = true;
volatile bool pan_homing_done = false;

void handleTimerEvent()
{
	if(tilt_homing_done && pan_homing_done)
	{
		u_tilt[2] = tilt_motor.getAngle();
		//u_pan[1] = pan_motor.getAngle();
		u_pan[2] = pan_motor.getAngle();

		// Tilt motor.
		if (!tilt_model.IsFinished())
		{
			tilt_model.Calculate(u_tilt, y_tilt);
			tilt_motor.setSpeedAndDirection(int8_t (y_tilt[0] * 100));

			if(tilt_model.IsFinished())
			{
				tilt_model.Terminate(u_tilt, y_tilt);
				tilt_motor.disable();
			}
		}

		// Pan motor.
		if (!pan_model.IsFinished())
		{
			pan_model.Calculate(u_pan, y_pan);
			//pan_motor.setSpeedAndDirection(int8_t (y_pan[1] * 100));
			pan_motor.setSpeedAndDirection(int8_t (y_pan[0] * 100));
		}
		else
		{
			pan_model.Terminate(u_pan, y_pan);
			pan_motor.disable();
		}
	}
	else
	{
		// Motor is still homing!
		if(tilt_motor.isAtInitialPosition() == true && !tilt_homing_done)
		{
			tilt_homing_done = true;
		}

		if(pan_motor.isAtInitialPosition() == true && !pan_homing_done)
		{
			pan_homing_done = true;
		}
	}
}

// Prevent optimization.
volatile float angle = 0;
void handleUartEvent(Serial::message &received_message)
{
	angle = received_message.getAngle();

	// Determine for which motor this message is intended.
	switch (received_message.motor)
	{
		case 0:
		{
			u_tilt[1] = angle;
			tilt_model.Initialize(u_tilt, y_tilt, 0.0);
			break;
		}

		case 1:
		{
			u_pan[1] = angle;
			pan_model.Initialize(u_pan, y_pan, 0.0);
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
	u_tilt[0] = 0.0;	/* corr */
	u_tilt[1] = 0.0;	/* in */
	u_tilt[2] = 0.0;	/* position */

	//u_pan[0] = 0.0;		/* in */
	//u_pan[2] = 0.0;		/* position */

	pan_model.SetFinishTime(1);
	tilt_model.SetFinishTime(1);

	// Let the motor run to the origin position.
	pan_motor.setSpeedAndDirection(-50);
	tilt_motor.setSpeedAndDirection(-50);

	usleep(10000);

	timer.enable();

	while(!tilt_homing_done || !pan_homing_done);

	pan_model.Initialize(u_pan, y_pan, 0.0);
	tilt_model.Initialize(u_tilt, y_tilt, 0.0);

	float prev_tilt = 0;
	float prev_pan = 0;

	while(1)
	{
		//u_tilt[1] += 3.14159265;
		//tilt_model.Initialize(u_tilt, y_tilt, 0.0);

		float tilt = tilt_motor.getAngle();

		if (tilt != prev_tilt)
		{
			prev_tilt = tilt;
			Serial::message message;
			message.motor = 0;
			message.setAngle(tilt);
			uart.sendData(message);
		}

		float pan = pan_motor.getAngle();

		if (pan != prev_pan)
		{
			prev_pan = pan;
			Serial::message message;
			message.motor = 1;
			message.setAngle(pan);
			uart.sendData(message);
		}
	}

	return 0;
}
