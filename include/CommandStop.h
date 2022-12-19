#pragma once
#include "Command.h"

class CommandStop : public Command
{
public:
    CommandStop(vector<string> args) : Command(args) {}
    virtual void execute(void);
};