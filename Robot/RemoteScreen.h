#pragma once

#include <Arduino.h>


class RemoteScreen
{
    public:
        void setup(int _address);

        bool send(String command);
        String receive(int quantity);

    private:
        int address;
};