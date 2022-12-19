#include <fstream>
#include <sstream>
#include "Environment.h"
#include "CommandMove.h"
#include "CommandMoveTo.h"
#include "CommandLiftFork.h"
#include "CommandTurn.h"
#include "CommandStop.h"
#include "CommandParallel.h"
#include "CommandLoop.h"
#include "CommandConditional.h"
#include "CommandRaiseError.h"
#include "CommandRunner.h"

// trim from start (in place)
static inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
        }).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
    rtrim(s);
    ltrim(s);
}

CommandRunner* CommandRunner::_instance = NULL;

void CommandRunner::ParseSpecFile(string specFile)
{
    std::ifstream specStream(specFile);
    Command* command = NULL;
    FactoryNoneReason reason;
    while ((command = CommandFactory(specStream, reason)) != NULL) {
        _commands.push_back(command);
    }
}

Command* CommandRunner::CommandFactory(std::ifstream& specStream, FactoryNoneReason& reason)
{
    Command* command = NULL;
    reason = FactoryNoneReason::FactoryNoneEOF;
    std::string line;
    do {
        if (!std::getline(specStream, line))
            return NULL;
        auto commentStart = line.find('#');
        if (commentStart != string::npos)
            line.erase(commentStart);
        trim(line);
        if (line.empty())
            continue;
        std::transform(line.begin(), line.end(), line.begin(),
            [](unsigned char c) { return std::tolower(c); });
        break;
    } while (true);

    stringstream ss(line);
    vector<string> tokens;
    string token;
    while (getline(ss, token, ' '))
        tokens.push_back(token);
    string opcode = tokens[0]; tokens.erase(tokens.begin());

    if (opcode == "move")
        command = new CommandMove(tokens);
    else if (opcode == "move-to")
        command = new CommandMoveTo(tokens);
    else if (opcode == "lift-fork")
        command = new CommandLiftFork(tokens);
    else if (opcode == "turn")
        command = new CommandTurn(tokens);
    else if (opcode == "stop")
        command = new CommandStop(tokens);
    else if (opcode == "if")
        command = new CommandConditional(tokens, specStream);
    else if (opcode == "parallel")
        command = new CommandParallel(tokens, specStream);
    else if (opcode == "loop")
        command = new CommandLoop(tokens, specStream);
    else if (opcode == "raise-error")
        command = new CommandRaiseError(tokens);
    else if (opcode == "else")
        reason = FactoryNoneReason::FactoryNoneElse;
    else if (opcode == "end")
        reason = FactoryNoneReason::FactoryNoneEnd;
    if (command)
    {
        command->setOpcode(opcode);
        reason = FactoryNoneReason::FactorySuccess;
    }

    return command;
}

CommandRunner::FactoryNoneReason CommandRunner::collectCommandBlock(std::ifstream& specStream, vector<Command*>& commands, FactoryNoneReason& reason)
{
    do {
        CommandRunner::FactoryNoneReason reason;
        Command* command = CommandRunner::GetInstance()->CommandFactory(specStream, reason);
        switch (reason)
        {
        case CommandRunner::FactoryNoneReason::FactorySuccess:
            commands.push_back(command);
            break;
        case CommandRunner::FactoryNoneReason::FactoryNoneEnd:
        case CommandRunner::FactoryNoneReason::FactoryNoneEOF:
            return reason;
        }
    } while (true);

    return reason;
}

void CommandRunner::executeSequence(vector<Command*> commands, bool isSequentialMode)
{
    _isSequentialMode = isSequentialMode;
    _sequences.push(_commands);
    _commands = commands;
    _iterCommands = _commands.begin();
}

void CommandRunner::executeSequence(void)
{
    do {
        _logger->debug("Executing sequence");
        while (!_commands.empty())
        {
            if (_isSequentialMode || _iterCommands == _commands.end())
                _iterCommands = _commands.begin();
            Command* c = *_iterCommands;

            if (c->getEvent() == Command::ExecEvents::EventExecutionEnded)
            {
                c->postExecute();
                _iterCommands = _commands.erase(_iterCommands);
                continue;
            }
            else
                _iterCommands++;
            c->preExecute();
            c->execute();
        }
        _logger->debug("Sequence complete");
        if (_sequences.size())
        {
            _commands = _sequences.top(); _sequences.pop();
            _iterCommands = _commands.begin();
            _isSequentialMode = true;
        }
    } while (_commands.size());
}

void CommandStop::execute(void) {
    CommandRunner::GetInstance()->cancelSequence();

    setEvent(Command::ExecEvents::EventExecutionEnded);
}

