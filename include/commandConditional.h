#pragma once
#include "Command.h"

class CommandConditional : public Command
{
private:
    bool _conditionTrue;
    vector<Command*> _commandsWhenTrue;
    vector<Command*> _commandsWhenFalse;

public:
    CommandConditional(vector<string> args, std::ifstream& specStream);
    static bool evaluateCondition(vector<string> args);
    virtual void preExecute(void) {
        _conditionTrue = evaluateCondition(_args);
    }
    virtual void execute(void);
};