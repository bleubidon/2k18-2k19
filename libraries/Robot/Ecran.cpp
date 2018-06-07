#include "Ecran.h"
	
#include <Wire.h>


void Ecran::setup(int _address)
{
	address = _address;
	
	Wire.begin();
}

bool Ecran::send(String command)
{
	Wire.beginTransmission(address);
	Wire.write(command.c_str());

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