#pragma once

// inspired by https://github.com/joshmarinacci/CmdArduino

#define MAX_ARGS 6
#define BUFFER_LENGTH 32

#include <HardwareSerial.h>


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