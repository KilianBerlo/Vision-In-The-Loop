#include "motor.hpp"

Plant::Motor::Motor(uint32_t base_address_first_word, uint32_t base_address_second_word, uint32_t frequency,
		uint32_t base_address_encoder, uint32_t steps_per_revolution) :
	base_address_first_word(base_address_first_word),
	base_address_second_word(base_address_second_word),
	frequency(frequency),
	base_address_encoder(base_address_encoder),
	radians_per_step((360.0 / float(steps_per_revolution)) * (3.14159265358979323846 / 180.0))
{
	disable();
	IOWR_32DIRECT(base_address_first_word, 0, frequency);
}


/*void  Plant::Motor::setSecondWord(uint32_t new_word)
{
	if (second_word != new_word)
	{
		IOWR_32DIRECT(base_address_second_word, 0, new_word);
		second_word = new_word;
	}
}*/

void Plant::Motor::setSpeedAndDirection(int8_t percentage)
{
	if (percentage == 0)
	{
		disable();
		return;
	}

	if (percentage > 0)
	{
		setDirection(Plant::Motor::Direction::CLOCKWISE);
	}
	if (percentage < 0)
	{
		setDirection(Plant::Motor::Direction::COUNTERCLOCKWISE);
		percentage = percentage * -1;
	}

	setDutyCycle(percentage);
    enable();
}

void Plant::Motor::setDutyCycle (uint8_t duty)
{
	duty_cycle = uint32_t ((float)frequency * ((float)duty / 100));
	IOWR_32DIRECT(base_address_second_word, 0, getSecondWord());
}

void Plant::Motor::setDirection(Direction dir)
{
	direction = static_cast<uint32_t>(dir);
	IOWR_32DIRECT(base_address_second_word, 0, getSecondWord());
}

void Plant::Motor::enable()
{
	en = 1;
	IOWR_32DIRECT(base_address_second_word, 0, getSecondWord());
}

void Plant::Motor::disable()
{
	en = 0;
	IOWR_32DIRECT(base_address_second_word, 0, getSecondWord());
}

float Plant::Motor::getAngle()
{
    return (readData() - encoder_start_position) * radians_per_step;
}

int32_t Plant::Motor::readData()
{
	return IORD_32DIRECT(base_address_encoder, 0);
}

