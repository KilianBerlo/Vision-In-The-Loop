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
Plant::Motor tilt_motor = Plant::Motor(MOTOR_PWM_0_SLAVE_1_BASE, MOTOR_PWM_0_SLAVE_2_BASE, PWM_FREQUENCY, QUADRATURE_ENCODER_0_BASE, 2000);
Plant::Motor pan_motor = Plant::Motor(MOTOR_PWM_1_SLAVE_1_BASE, MOTOR_PWM_1_SLAVE_2_BASE, PWM_FREQUENCY, QUADRATURE_ENCODER_1_BASE, 4915);

// Motor input and output value arrays.
XXDouble u_tilt [3 + 1];
XXDouble y_tilt [1 + 1];

XXDouble u_pan [3 + 1];
XXDouble y_pan [1 + 1];

// Definitions of 20-sim models.
PositionControllerTilt tilt_model;
PositionControllerTilt pan_model;

volatile bool tilt_homing_done = false;
volatile bool pan_homing_done = false;

void handleTimerEvent()
{
	if(tilt_homing_done && pan_homing_done)
	{
		u_tilt[2] = tilt_motor.getAngle();
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
			u_tilt[1] += angle;

			if (u_tilt[1] > 0.1 && u_tilt[1] < 3)
			{
				tilt_model.Initialize(u_tilt, y_tilt, 0.0);
			}

			break;
		}

		case 1:
		{
			u_pan[1] += angle;

			if (u_pan[1] > 0.1 && u_pan[1] < 5.5)
			{
				pan_model.Initialize(u_pan, y_pan, 0.0);
			}

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
	u_tilt[1] = 1.4;	/* in */
	u_tilt[2] = 0.0;	/* position */

	u_pan[0] = 0.0;		/* corr */
	u_pan[1] = 2.9;		/* in */
	u_pan[2] = 0.0;		/* position */

	pan_model.SetFinishTime(1);
	tilt_model.SetFinishTime(1);

	// Let the motor run to the origin position.
	pan_motor.setSpeedAndDirection(-50);
	tilt_motor.setSpeedAndDirection(-50);

	usleep(10000);

	// Enable the timer interrupts.
	timer.enable();

	// Wait until the homing is done.
	while(!tilt_homing_done || !pan_homing_done);

	// Wait for 100ms before starting the controller.
	usleep(100000);

	// Move to the middle of both axis.
	pan_model.Initialize(u_pan, y_pan, 0.0);
	tilt_model.Initialize(u_tilt, y_tilt, 0.0);

	while(1)
	{
		// Just a 100ms delay. Everything is interrupt based!
		usleep(100000);
	}

	return 0;
}
