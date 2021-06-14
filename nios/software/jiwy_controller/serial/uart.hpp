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
		int32_t encoder_value : 31;
		int32_t motor : 1;

		tx_message(int32_t encoder_value, int32_t motor_value) :
				encoder_value(encoder_value),
				motor (motor_value)
		{}
	};

	class UART
	{

	public:
		UART(std::function<void(Serial::rx_message &received_message)> callback);

		void sendData(tx_message msg);

	private:

		std::array<uint8_t, 4> convertToArray(tx_message msg);

		static bool uart_ready;
		static std::array<uint8_t, MESSAGE_SIZE> rx_data;
		static alt_u8 rx_len;
		static std::function<void(Serial::rx_message &received_message)> callback;

		static void uart_0_isr(void *);
	};


}

#endif /* SERIAL_UART_HPP_ */
