#ifndef MOTOR_DRIVER_MOTOR_HPP_
#define MOTOR_DRIVER_MOTOR_HPP_

#include <stdio.h>
#include <io.h>
#include "system.h"


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

			Motor(uint32_t base_address_first_word, uint32_t base_address_second_word, uint32_t frequency,
					uint32_t base_address_encoder, uint32_t steps_per_revolution);

			void setSpeedAndDirection(int8_t percentage);

	        void setDutyCycle (uint8_t duty);

	        void setDirection(Direction direction);

	        void enable();

			void disable();

			float getAngle();

			bool isAtInitialPosition()
			{
				int32_t value = readData();

				if (value != encoder_start_position)
				{
					encoder_start_position = value;
					return false;
				}
				else
				{
					encoder_start_position = value;
					return true;
				}
			}

			int32_t encoder_start_position = 0;

			//void setSecondWord(uint32_t new_word);

		private:
			const uint32_t base_address_first_word;
			const uint32_t base_address_second_word;
			const uint32_t frequency = 0;
			const uint32_t base_address_encoder;
			const float radians_per_step;

			int32_t readData();



			uint32_t getSecondWord()
			{
				return (duty_cycle << 16) | (direction << 14) | en << 13;
			}


			uint32_t duty_cycle = 0;
			uint32_t direction = 0;
			uint32_t en = 0;
	};

}

#endif /* MOTOR_DRIVER_MOTOR_HPP_ */
