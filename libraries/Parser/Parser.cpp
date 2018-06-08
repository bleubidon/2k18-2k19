#include "Parser.h"

#include <string.h>


struct Command
{
	const char* name;
	void (*func)(int argc, char **argv);

	Command* next;
};
		
Command* list = NULL;



Parser::Parser():
	cursor(0)
{ }


void Parser::setup(Stream* _stream)
{
	if (_stream == nullptr)
		stream = &Serial;
	else
		stream = _stream;
}

void Parser::loop()
{
	while (stream->available())
	{
		char c = stream->read();

		if (c == '\n')
		{
			buffer[cursor] = '\0';
			cursor = 0;

			parse(buffer);
		}
		else if (cursor < BUFFER_LENGTH)
			buffer[cursor++] = c;
	}
}

void Parser::add(const char* name, void (*func)(int argc, char **argv))
{
	Command* cmd = new Command();
		cmd->name = name;
		cmd->func = func;
		cmd->next = list;

	list = cmd;
}


bool Parser::parse(char* command)
{
	int argc, i = 0;
	char *argv[MAX_ARGS];


	argv[0] = strtok(command, " ");
	do {
		argv[++i] = strtok(NULL, " ");
	}
	while ((i < MAX_ARGS) && (argv[i] != NULL));

	argc = i;


	for (Command* cmd = list; cmd != NULL; cmd = cmd->next)
	{
		if (!strcmp(argv[0], cmd->name))
		{
			cmd->func(argc, argv);
			return true;
		}
	}

	stream->print("parser: ");
	stream->print(argv[0]);
	stream->println(": command not found");
	
	return false;
}

bool Parser::parse(const char* command)
{
	strcpy(buffer, command);
	return parse(buffer);
}