#pragma once
#include <numeric>
#include "Command.h"

class CommandRaiseError : public Command
{
public:
    CommandRaiseError(vector<string> args) : Command(args) {
        _logger = CLogger::GetInstance()->CreateLogger(typeid(*this).name());
    }
    virtual void execute(void) {
        std::string s;
        std::for_each(_args.begin(), _args.end(), [&](const std::string& piece) { s += piece + " "; });
        _logger->debug("Raise-Error: {}", s);

        setEvent(Command::ExecEvents::EventExecutionEnded);
    }
};