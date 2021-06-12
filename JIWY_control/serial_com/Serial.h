//
// Created by glen on 07-06-21.
//

#ifndef JIWY_CONTROL_SERIAL_H
#define JIWY_CONTROL_SERIAL_H


#include <string>
#include <array>
#include <optional>

class Serial
{
    static constexpr uint8_t MESSAGE_SIZE = 4;

    struct rx_message
    {
        int32_t encoder_value : 31;
        uint32_t motor : 1;
    };


    public:
        Serial(std::string com_port);

    std::optional<rx_message> readMessage(bool await_response = false);
        void write_array(std::array<uint8_t, 4> data);

    private:

        int file;
};


#endif //JIWY_CONTROL_SERIAL_H
