#pragma once

// inspired by https://github.com/joshmarinacci/CmdArduino

#define MAX_ARGS 6
#define MAX_COMMAND_LENGTH  30

struct Command
{
    const char* name;
    void (*func)(int argc, char **argv);

    Command* next;
};

class Parser
{
    public:
        Parser();
        ~Parser();

        void add(const char* name, void (*func)(int argc, char **argv));

        void parse(char* command);
        void parse(const char* command);
        
        void loop();

    private:
        Command* list;
        
        char buffer[MAX_COMMAND_LENGTH];
		char* cursor;
};