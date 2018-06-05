#pragma once

#include <Arduino.h>


class Ecran
{
    public:
        void setup(byte _address);

        bool send(String command);
        String receive(int quantity);

    private:
        byte address;
};