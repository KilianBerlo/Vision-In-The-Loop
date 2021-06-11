#include "uart.hpp"

bool Serial::UART::uart_ready = false;
std::array<uint8_t, Serial::MESSAGE_SIZE> Serial::UART::rx_data;
alt_u8 Serial::UART::rx_len = 0;
std::function<void(Serial::rx_message &received_message)> Serial::UART::callback;

void Serial::UART::uart_0_isr(void *)
{
	// Check if the UART buffer is empty or if there are more messages waiting.
	alt_u16 status = IORD_ALTERA_AVALON_UART_STATUS(UART_0_BASE);

	if(status & ALTERA_AVALON_UART_STATUS_RRDY_MSK)
	{
		rx_data[rx_len] = IORD_ALTERA_AVALON_UART_RXDATA(UART_0_BASE);
		rx_len ++ ;

		if (rx_len >= MESSAGE_SIZE)
		{
			uart_ready = true;
			rx_len = 0;
		}
	}

	if (uart_ready)
	{
		uart_ready = 0;

		Serial::rx_message received_message;
		std::memcpy(&received_message, &rx_data, MESSAGE_SIZE);

		callback(received_message);
	}
}

Serial::UART::UART(std::function<void(Serial::rx_message &received_message)> func)
{
	callback = func;

	// Clear status flag.
	IOWR_ALTERA_AVALON_UART_STATUS(UART_0_BASE, 0x00);

	// Read the useless value in the empty receive register.
	IORD_ALTERA_AVALON_UART_RXDATA(UART_0_BASE);

	// Setup RX IRQ routine.
	alt_u16 control = ALTERA_AVALON_UART_CONTROL_RRDY_MSK | ALTERA_AVALON_UART_CONTROL_E_MSK;
	IOWR_ALTERA_AVALON_UART_CONTROL (UART_0_BASE, control);

	// Install IRQ service routine.
    alt_ic_isr_register(UART_0_IRQ_INTERRUPT_CONTROLLER_ID, UART_0_IRQ, uart_0_isr, NULL, NULL);

    // Enable interrupt.
	IOWR_ALTERA_AVALON_UART_CONTROL(UART_0_BASE, ALTERA_AVALON_UART_CONTROL_RRDY_MSK);
}


void Serial::UART::sendData(std::array<uint8_t, 4> data)
{
	for (uint8_t data_byte : data)
	{
		alt_u8 status = IORD_ALTERA_AVALON_UART_STATUS (UART_0_BASE);

		// Wait for sending to finish.
		while (! (status & ALTERA_AVALON_UART_STATUS_TRDY_MSK))
		{
			status = IORD_ALTERA_AVALON_UART_STATUS (UART_0_BASE);
		}

		IOWR_ALTERA_AVALON_UART_TXDATA(UART_0_BASE, data_byte);
	}
}
