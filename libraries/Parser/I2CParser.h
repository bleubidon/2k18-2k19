#pragma once

#include <Arduino.h>
#include "Parser.h"

class I2CParser: public Parser
{
    public:
        void setup();
        void setup(uint8_t _address);

        bool parse(uint8_t _address, char* command);
        bool parse(uint8_t _address, const char* command);
        
        String request(uint8_t quantity);
        
        void loop();
        void loop(uint8_t _address);

    private:
        uint8_t address;
};