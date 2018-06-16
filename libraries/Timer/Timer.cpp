#include "Timer.h"
#include <Arduino.h>

Timer::Timer(unsigned long _period) :
    last(0), period(_period)
{ }

bool Timer::on()
{
    unsigned long current = millis();

    if (current - last < period)
        return false;

    last = current;
    return true;
}