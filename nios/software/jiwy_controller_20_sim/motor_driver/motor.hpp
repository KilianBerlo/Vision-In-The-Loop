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
			Motor(uint32_t base_address_first_word, uint32_t base_address_second_word);

			void stop();

			void setFrequency(uint32_t new_frequency);
			void setSecondWord(uint32_t new_word);

		private:
			const uint32_t base_address_first_word;
			const uint32_t base_address_second_word;

			uint32_t frequency = 0;
			uint32_t second_word = 0;
	};

}

#endif /* MOTOR_DRIVER_MOTOR_HPP_ */
