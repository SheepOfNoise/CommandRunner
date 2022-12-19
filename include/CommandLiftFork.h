#pragma once
#include "Command.h"

class CommandLiftFork : public Command
{
public:
    CommandLiftFork(vector<string> args) : Command(args) {
        _logger = CLogger::GetInstance()->CreateLogger(typeid(*this).name());
        preExecute();
    }
    virtual void preExecute(void) {
        if (!_preExecuted)
            Environment::GetInstance()->setVarTarget(Environment::StateVariables::StateY, atoi(_args[0].c_str()));
        _preExecuted = true;
    }
    virtual void execute(void) {
        Command::ExecEvents event;
        handleEvent(event = Environment::GetInstance()->WaitForEvent(Environment::StateVariables::StateY));
        setEvent(event);
    }
};