#include "Parser.h"

#include <Arduino.h>
#include <string.h>

// Serial print helpers
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }
const char endl = '\n';


Parser::Parser():
    list(NULL)
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

void Parser::parse(char* command)
{
    static const int maxArg = 6;

    int argc, i = 0;
    char *argv[maxArg];


    argv[0] = strtok(command, " ");
    do {
        argv[++i] = strtok(NULL, " ");
    }
    while ((i < maxArg) && (argv[i] != NULL));

    argc = i;
    

    for (Command* cmd = list; cmd != NULL; cmd = cmd->next)
    {
        if (!strcmp(argv[0], cmd->name))
        {
            cmd->func(argc, argv);
            return;
        }
    }

    // command not recognized
    Serial << "parser: " << argv[0] << ": command not found" << endl;
}

void Parser::parse(const char* command)
{
    char* copy = strdup(command);
    parse(copy);
}