#pragma once
#include "Command.h"

class CommandParallel : public Command
{
public:
    CommandParallel(vector<string> args, std::ifstream& specStream);
    virtual void execute(void);
private:
    vector<Command*> _subCommands;
};