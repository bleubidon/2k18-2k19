#pragma once

#include "Parser.h"
#include <stdint.h>


class I2CParser: public Parser
{
	public:
		/*** Master ***/
		void setup(Stream* _stream = nullptr);
		void loop(uint8_t _address);

		bool write(uint8_t _address, char* command);
		bool write(uint8_t _address, const char* command);

		char* read(uint8_t _address, uint8_t quantity);

		/*** Slave ***/
		void setup(uint8_t _address, Stream* _stream = nullptr);
		void loop();

		void setAnswer(char* _answer);


	private:
		char answer[BUFFER_LENGTH];
		uint8_t address;
};
