#include "encoder.hpp"

Plant::Encoder::Encoder(uint32_t base_address) :
	base_address(base_address)
{}

int32_t Plant::Encoder::readData()
{
	return IORD_32DIRECT(base_address, 0);
}
