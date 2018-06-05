#pragma once

// inspired by https://github.com/joshmarinacci/CmdArduino

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

    private:
        Command* list;
};