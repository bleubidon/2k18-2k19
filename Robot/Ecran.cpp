#include "Sick.h"
    
#include "Arduino.h"


void Ecran::setup(byte _address)
{
	address = _address;
	
    Wire.begin();
}

bool Ecran::send(String command)
{
    Wire.beginTransmission(address);
    Wire.write(command);

    return Wire.endTransmission() == 0; // 0 means success
}

String Ecran::receive(int quantity)
{
    Wire.requestFrom(address, quantity);

	String answer;
	int length = Wire.available();
    if (length)
    {
		answer.reserve(length);

		while (length--)
			answer.concat(Wire.read());
    }
        
	return answer;
}