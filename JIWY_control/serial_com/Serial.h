//
// Created by glen on 07-06-21.
//

#ifndef JIWY_CONTROL_SERIAL_H
#define JIWY_CONTROL_SERIAL_H


#include <string>
#include <array>

class Serial
{
    public:
        Serial(std::string com_port);

    std::vector<uint8_t> read_array(int lenght, bool await_response = false);
        void write_array(std::array<uint8_t, 4> data);

    private:

        int file;
};


#endif //JIWY_CONTROL_SERIAL_H
