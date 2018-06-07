#pragma once

// inspired by https://github.com/joshmarinacci/CmdArduino

#define MAX_ARGS 6
#define BUFFER_LENGTH 32

class Parser
{
	public:
		Parser();

		void add(const char* name, void (*func)(int argc, char **argv));

		bool parse(char* command);
		bool parse(const char* command);

		virtual void loop();

	protected:
		char buffer[BUFFER_LENGTH];
		int cursor;
};