#ifndef MOTOR_DRIVER_ENCODER_HPP_
#define MOTOR_DRIVER_ENCODER_HPP_

#include <stdio.h>
#include <io.h>

namespace Plant
{
	class Encoder
	{
	public:
		Encoder(uint32_t base_address);

		int32_t readData();

	private:
		const uint32_t base_address;
	};
}

#endif /* MOTOR_DRIVER_ENCODER_HPP_ */
