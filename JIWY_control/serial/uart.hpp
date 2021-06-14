#ifndef JIWY_CONTROL_UART_HPP
#define JIWY_CONTROL_UART_HPP

#include <string>
#include <array>
#include <optional>

namespace Serial
{
    static constexpr uint8_t MESSAGE_SIZE = 4;

    struct tx_message
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
    };

    struct rx_message
    {
        int32_t encoder_value : 31;
        uint32_t motor : 1;
    };

    class UART
    {
    public:
        UART(std::string com_port);

        std::optional<rx_message> readMessage(bool await_response = false);

        void writeMessage(Serial::tx_message msg);

    private:

        std::array<uint8_t, 4> convertToArray(Serial::tx_message msg);

        int file;
    };
}




#endif //JIWY_CONTROL_UART_HPP
