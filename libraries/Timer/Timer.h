#pragma once

struct Timer
{
    Timer(unsigned long _period);
    bool on();
    
    unsigned long last, period;
};