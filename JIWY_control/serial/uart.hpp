#ifndef JIWY_CONTROL_UART_HPP
#define JIWY_CONTROL_UART_HPP

#include <cstdint>
#include <cstring>
#include <string>
#include <array>
#include <optional>

namespace Serial
{
    static constexpr uint8_t MESSAGE_SIZE = 4;

    struct message
    {
        uint32_t encoder_value : 31;
        uint32_t motor : 1;

        void setAngle(float angle)
        {
            unsigned r;
            std::memcpy(&r,&angle,sizeof r);
            encoder_value = r >> 1;
        }

        [[nodiscard]] float getAngle() const
        {
            unsigned int x = encoder_value;
            x <<= 1;
            float r;
            std::memcpy(&r,&x,sizeof r);
            return r;
        }
    };

    class UART
    {
    public:
        UART(std::string com_port);

        std::optional<message> readMessage(bool await_response = false);

        void writeMessage(Serial::message msg);

    private:

        std::array<uint8_t, 4> convertToArray(Serial::message msg);

        int file;

    };
}




#endif //JIWY_CONTROL_UART_HPP
