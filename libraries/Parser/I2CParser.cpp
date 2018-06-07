#include "I2CParser.h"

#include <Wire.h>

// Serial print helpers
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }
const char endl = '\n';


char* I2CBuffer;
int* I2Ccursor;

void _receiveEvent(int);
void _requestEvent();


void I2CParser::setup()
{
	I2CBuffer = buffer;
	I2Ccursor = &cursor;

	buffer[0] = '\0';

	Wire.begin();
}

void I2CParser::setup(uint8_t _address)
{
	I2CBuffer = buffer;
	I2Ccursor = &cursor;
	
	address = _address;
	buffer[0] = '\0';
	
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
	strcpy(buffer, command);
	return parse(_address, buffer);
}


void I2CParser::setAnswer(char* _answer)
{
	strcpy(buffer, _answer);
}

char* I2CParser::requestFrom(uint8_t _address, uint8_t quantity)
{
	cursor = 0;
	Wire.requestFrom(_address, quantity);

	while (Wire.available())
		buffer[cursor++] = Wire.read();

	buffer[cursor] = '\0';
	return buffer;
}

void I2CParser::loop()
{
	if (buffer[0] != '\0')
	{
		Parser::parse(buffer);
		buffer[0] = '\0';
	}
}

void I2CParser::loop(uint8_t _address)
{
	while (Serial.available())
	{
		char c = Serial.read();

		if (c == '\n')
		{
			buffer[cursor] = '\0';
			cursor = 0;

			parse(_address, buffer);
		}
		else if (cursor < BUFFER_LENGTH)
			buffer[cursor++] = c;
	}
}

// Callbacks
void _receiveEvent(int length)
{
	*I2Ccursor = 0;
	while (Wire.available() && *I2Ccursor < BUFFER_LENGTH)
		I2CBuffer[*I2Ccursor++] = Wire.read();

	I2CBuffer[*I2Ccursor] = '\0';
}

void _requestEvent()
{
	Wire.write(I2CBuffer);
}