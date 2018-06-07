#include "I2CParser.h"
    
#include <Wire.h>

// Serial print helpers
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }
const char endl = '\n';


char I2Crequest[BUFFER_LENGTH];
char I2Canswer[BUFFER_LENGTH];

void _receiveEvent(int);
void _requestEvent();


void I2CParser::setup()
{
	*I2Crequest = '\0';
	*I2Canswer = '\0';

    Wire.begin();
}

void I2CParser::setup(uint8_t _address)
{
	address = _address;
	*I2Crequest = '\0';
	*I2Canswer = '\0';
	
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
    bool ret = parse(_address, copy);

	free(copy);
	return ret;
}


void I2CParser::setAnswer(char* _answer)
{
	strcpy(I2Canswer, _answer);
}

char* I2CParser::requestFrom(uint8_t _address, uint8_t quantity)
{
	int i = 0;
    Wire.requestFrom(_address, quantity);

	while (Wire.available())
		I2Canswer[i++] = Wire.read();
        
	I2Canswer[i] = '\0';
	return I2Canswer;
}

void I2CParser::loop()
{
	if (*I2Crequest != '\0')
	{
		Parser::parse(I2Crequest);
		*I2Crequest = '\0';
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
		I2Crequest[i++] = Wire.read();

	I2Crequest[i] = '\0';
}

void _requestEvent()
{
	Wire.write(I2Canswer);
	I2Canswer[0] = '\0';
}