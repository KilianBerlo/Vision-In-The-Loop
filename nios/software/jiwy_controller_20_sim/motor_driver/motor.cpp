#include "motor.hpp"

Plant::Motor::Motor(uint32_t base_address_first_word, uint32_t base_address_second_word) :
	base_address_first_word(base_address_first_word),
	base_address_second_word(base_address_second_word)
{
	stop();
}

void Plant::Motor::setFrequency(uint32_t new_frequency)
{
	if (frequency != new_frequency)
	{
		IOWR_32DIRECT(base_address_first_word, 0, new_frequency);
		frequency = new_frequency;
	}
}

void  Plant::Motor::setSecondWord(uint32_t new_word)
{
	if (second_word != new_word)
	{
		IOWR_32DIRECT(base_address_second_word, 0, new_word);
		second_word = new_word;
	}
}

void Plant::Motor::stop()
{
	// Set all values to zero.
	IOWR_32DIRECT(base_address_first_word, 0, 0);
	IOWR_32DIRECT(base_address_second_word, 0, 0);
}

