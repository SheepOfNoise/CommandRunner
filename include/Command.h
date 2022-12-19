#pragma once

#include <vector>
#include <string>
#include "CLogger.h"
using namespace std;

class Command
{
public:
    enum class ExecEvents
    {
        EventExecutionStarted,
        EventExecutionProgress,
        EventExecutionEnded
    };

    Command(vector<string> args) : _args(args), _lastEvent(ExecEvents::EventExecutionStarted), _preExecuted(false) { };
    virtual void preExecute(void) {}
    virtual void execute(void) = 0;
    virtual void postExecute(void) {}
    virtual void handleEvent(ExecEvents) {};
    void setOpcode(string opcode) { _opcode = opcode; }
    void setEvent(Command::ExecEvents event) { _lastEvent = event; }
    Command::ExecEvents getEvent() { return _lastEvent; }

protected:
    std::shared_ptr<spdlog::logger> _logger;
    vector<string> _args;
    string _opcode;
    bool _preExecuted;
private:
    Command::ExecEvents _lastEvent;
};
