#pragma once

#include <stack>
#include "Command.h"

class CommandRunner
{
public:
    enum class FactoryNoneReason {
        FactorySuccess,
        FactoryNoneEOF,
        FactoryNoneElse,
        FactoryNoneEnd
    };

    CommandRunner(const CommandRunner&) = delete;
    CommandRunner& operator=(const CommandRunner&) = delete;
    static CommandRunner* GetInstance()
    {
        if (_instance == 0)
            _instance = new CommandRunner();
        return _instance;
    }

    void ParseSpecFile(string sequence_spec_file);
    Command* CommandFactory(std::ifstream&, FactoryNoneReason&);
    static FactoryNoneReason collectCommandBlock(std::ifstream& specStream, vector<Command*>& commands, FactoryNoneReason& reason);
    void executeSequence(void);
    void executeSequence(vector<Command*>, bool isSequentialMode = true);
    void cancelSequence(void) {
        _commands.clear();
        _logger->warn("Current sequence cancelled");
    }

private:
    static CommandRunner* _instance;
    std::shared_ptr<spdlog::logger> _logger;
    stack<vector<Command*>> _sequences;
    vector<Command*> _commands;
    vector<Command*>::iterator _iterCommands;
    bool _isSequentialMode;

    CommandRunner() : _isSequentialMode(true) {
        _logger = CLogger::GetInstance()->CreateLogger("Runner");
    }

};
