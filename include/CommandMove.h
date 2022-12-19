#pragma once
#include "Command.h"
#include "Environment.h"

class CommandMove : public Command
{
public:
    CommandMove(vector<string> args) : Command(args) {
        _logger = CLogger::GetInstance()->CreateLogger(typeid(*this).name());
    }
    virtual void preExecute(void) {
        if (!_preExecuted)
            Environment::GetInstance()->setVarTarget(Environment::StateVariables::StateZ, atoi(_args[0].c_str()));
        _preExecuted = true;
    }
    virtual void execute(void) {
        Command::ExecEvents event;
        handleEvent(event = Environment::GetInstance()->WaitForEvent(Environment::StateVariables::StateZ));
        setEvent(event);
    }
};