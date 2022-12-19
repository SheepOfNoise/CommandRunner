#pragma once
#include "Command.h"

class CommandLoop : public Command
{
public:
    CommandLoop(vector<string> args, std::ifstream& specStream);
    virtual void execute(void);
private:
    int _iterations;
    vector<Command*> _subCommands;
};
