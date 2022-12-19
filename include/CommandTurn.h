#pragma once
#include "Command.h"
#include "Environment.h"

class CommandTurn : public Command
{
public:
    CommandTurn(vector<string> args) : Command(args) {
        _logger = CLogger::GetInstance()->CreateLogger(typeid(*this).name());
    }
    virtual void preExecute(void) {
        if (!_preExecuted)
        {
            int diff = (_args[0] == "right" ? 1 : -1) * atoi(_args[1].c_str());
            int current = Environment::GetInstance()->getCurrentValue(Environment::StateVariables::StateX);
            Environment::GetInstance()->setVarTarget(Environment::StateVariables::StateX, (current + diff + 360) % 360);
        }
        _preExecuted = true;
    }
    virtual void execute(void);
};