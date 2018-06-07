#pragma once

#include <Arduino.h>


class Ecran
{
    public:
        void setup(int _address);

        bool send(String command);
        String receive(int quantity);

    private:
        int address;
};