#pragma once
#include "Command.h"
#include "Environment.h"

class CommandMoveTo : public Command
{
public:
    CommandMoveTo(vector<string> args) : Command(args) {
        _logger = CLogger::GetInstance()->CreateLogger(typeid(*this).name());
        _targetX = atof(_args[0].c_str());
        _targetY = atof(_args[1].c_str());
    }
    virtual void preExecute(void);
    virtual void execute(void);

private:
    vector<Command*> _legCommands;
    double _targetX, _targetY;

    double calcDistance(void);
    bool isAtTarget(void) {
        return calcDistance() < 2.0;
    }
};