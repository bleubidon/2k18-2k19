#include "I2CParser.h"

#include <Wire.h>


static char* I2CBuffer;
static char* I2CAnswer;
static int* I2CCursor;

static void _receiveEvent(int);
static void _requestEvent();


/*** Master ***/

void I2CParser::setup(Stream* _stream)
{
	Parser::setup(_stream);

	I2CBuffer = buffer;
	I2CAnswer = answer;
	I2CCursor = &cursor;

	Wire.begin();
}

void I2CParser::loop(uint8_t _address)
{
	while (stream->available())
	{
		char c = stream->read();

		if (c == '\n')
		{
			buffer[cursor] = '\0';
			cursor = 0;

			write(_address, buffer);
		}
		else if (cursor < BUFFER_LENGTH)
			buffer[cursor++] = c;
	}
}


bool I2CParser::write(uint8_t _address, char* command)
{
	Wire.beginTransmission(_address);
	Wire.write(command);

	return Wire.endTransmission() == 0; // 0 means success
}

bool I2CParser::write(uint8_t _address, const char* command)
{
	Wire.beginTransmission(_address);
	Wire.write(command);

	return Wire.endTransmission() == 0; // 0 means success
}


char* I2CParser::read(uint8_t _address, uint8_t quantity)
{
	int i = 0;
	Wire.requestFrom(_address, quantity);

	while (Wire.available())
		answer[i++] = Wire.read();

	answer[i] = '\0';
	return answer;
}


/*** Slave ***/

void I2CParser::setup(uint8_t _address, Stream* _stream)
{
	Parser::setup(_stream);

	I2CBuffer = buffer;
	I2CAnswer = answer;
	I2CCursor = &cursor;
	
	address = _address;
	
	Wire.begin(_address);
	Wire.onReceive(_receiveEvent);
	Wire.onRequest(_requestEvent);
}

void I2CParser::loop()
{
	if (buffer[0] != '\0')
	{
		Parser::parse(buffer);
		buffer[0] = '\0';
	}
}


void I2CParser::setAnswer(char* _answer)
{
	strcpy(answer, _answer);
}

// Callbacks
static void _receiveEvent(int length)
{
	*I2CCursor = 0;
	while (Wire.available() && *I2CCursor < BUFFER_LENGTH)
		I2CBuffer[(*I2CCursor)++] = Wire.read();

	I2CBuffer[*I2CCursor] = '\0';
}

static void _requestEvent()
{
	Wire.write(I2CAnswer);
}