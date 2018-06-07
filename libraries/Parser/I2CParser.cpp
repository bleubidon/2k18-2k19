#include "I2CParser.h"
    
#include <Wire.h>

// Serial print helpers
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }
const char endl = '\n';


char I2Cbuffer[BUFFER_LENGTH];
void _receiveEvent(int);
void _requestEvent();


void I2CParser::setup()
{
	*I2Cbuffer = '\0';

    Wire.begin();
}

void I2CParser::setup(uint8_t _address)
{
	address = _address;
	*I2Cbuffer = '\0';
	
    Wire.begin(_address);
	Wire.onReceive(_receiveEvent);
	Wire.onRequest(_requestEvent);
}


bool I2CParser::parse(uint8_t _address, char* command)
{
    Wire.beginTransmission(_address);
    Wire.write(command);

    return Wire.endTransmission() == 0; // 0 means success
}

bool I2CParser::parse(uint8_t _address, const char* command)
{
    char* copy = strdup(command);
    return parse(_address, copy);
}


String I2CParser::request(uint8_t quantity)
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

void I2CParser::loop()
{
	if (*I2Cbuffer != '\0')
	{
		Parser::parse(I2Cbuffer);
		*I2Cbuffer = '\0';
	}
}

void I2CParser::loop(uint8_t _address)
{
	while (Serial.available())
	{
		char c = Serial.read();

		if (c == '\n')
		{
            *cursor = '\0';
            cursor = buffer;

			parse(_address, buffer);
		}
		else
			*cursor++ = c;
	}
}

// Callbacks
void _receiveEvent(int length)
{
	int i = 0;
	while (Wire.available() && i < BUFFER_LENGTH)
		I2Cbuffer[i++] = Wire.read();

	I2Cbuffer[i] = '\0';
	Serial << "parser: " << I2Cbuffer << endl;
}

void _requestEvent()
{
	char response = '1';
	Wire.write(response);
	Serial << "Answering |" << response << "|   ASCII: " << (int)response << endl;
}