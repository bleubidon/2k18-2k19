#pragma once

#include <HardwareSerial.h>

// inspired by https://github.com/joshmarinacci/CmdArduino

const int MAX_ARGS = 6;
const int BUFFER_LENGTH = 32;


class Parser
{
	public:
		Parser();

		void setup(Stream* _stream = nullptr);
		void loop();

		void add(const char* name, void (*func)(int argc, char **argv));

		bool parse(char* command);
		bool parse(const char* command);

	protected:
		Stream* stream;

		char buffer[BUFFER_LENGTH];
		int cursor;
};
