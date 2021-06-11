#ifndef SERIAL_UART_HPP_
#define SERIAL_UART_HPP_

#include <stdio.h>
#include <array>
#include <functional>
#include <cstring>

#include "altera_avalon_uart_regs.h"
#include "sys/alt_irq.h"

namespace Serial
{
	static constexpr uint8_t MESSAGE_SIZE = 4;

	struct rx_message
	{
		uint32_t freq : 14;
		uint32_t duty : 14;
		uint32_t direction : 2;
	    uint32_t enable : 1;
	    uint32_t motor : 1;

		void setDutyCycle (uint8_t duty_cycle)
		{
			duty = uint32_t ((float)freq * ((float)duty_cycle / 100));
		}

		uint32_t getFullField()
		{
			return (freq << 18) | (duty << 4) | (direction << 2) | (enable << 1) | motor;
		}

		uint32_t getFirstWord()
		{
			return freq;
		}

		uint32_t getSecondWord()
		{
			return (duty << 16) | (direction << 14) | enable << 13;
		}
	};

	struct tx_message
	{
		uint32_t encoder_value;

		tx_message(uint32_t value) :
				encoder_value(value)
		{}

		std::array<uint8_t, MESSAGE_SIZE> getAsArray()
		{
			std::array<uint8_t, MESSAGE_SIZE> temp{};
		    std::copy(
		            reinterpret_cast<uint8_t *>(&encoder_value),
		            reinterpret_cast<uint8_t *>(&encoder_value) + MESSAGE_SIZE,
		            &temp[0]
		    );
		    return temp;
		}
	};

	class UART
	{

	public:
		UART(std::function<void(Serial::rx_message &received_message)> callback);

		void sendData(std::array<uint8_t, MESSAGE_SIZE> data);

	private:

		static bool uart_ready;
		static std::array<uint8_t, MESSAGE_SIZE> rx_data;
		static alt_u8 rx_len;
		static std::function<void(Serial::rx_message &received_message)> callback;

		static void uart_0_isr(void *);
	};


}

#endif /* SERIAL_UART_HPP_ */
