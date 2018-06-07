#include "Parser.h"

#include <Arduino.h>
#include <string.h>

// Serial print helpers
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }
const char endl = '\n';


Parser::Parser():
    list(NULL), cursor(buffer)
{
}

Parser::~Parser()
{
    if (list == NULL)
        return;

    Command* curr = list;

    do {
        Command* next = curr->next;

        delete curr;
        curr = next;
    }
    while (curr != NULL);
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

    // command not recognized
    Serial << "parser: " << argv[0] << ": command not found" << endl;
    return false;
}

bool Parser::parse(const char* command)
{
    char* copy = strdup(command);
    bool ret = parse(copy);

	free(copy);
	return ret;
}

void Parser::loop()
{
	while (Serial.available())
	{
		char c = Serial.read();

		if (c == '\n')
		{
            *cursor = '\0';
            cursor = buffer;

			parse(buffer);
		}
		else
			*cursor++ = c;
	}
}