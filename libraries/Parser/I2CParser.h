#pragma once

#include <Arduino.h>
#include "Parser.h"

class I2CParser: public Parser
{
	public:
		void setup();
		void setup(uint8_t _address);

		bool parse(uint8_t _address, char* command);
		bool parse(uint8_t _address, const char* command);
		
		void setAnswer(char* _answer);
		char* requestFrom(uint8_t _address, uint8_t quantity);
		
		void loop();
		void loop(uint8_t _address);

	private:
		char answer[BUFFER_LENGTH];
		uint8_t address;
};