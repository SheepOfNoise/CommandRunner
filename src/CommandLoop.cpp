#include "CommandLoop.h"
#include "CommandConditional.h"
#include "CommandRunner.h"

CommandLoop::CommandLoop(vector<string> args, std::ifstream& specStream) : Command(args) {
    _logger = CLogger::GetInstance()->CreateLogger(typeid(*this).name());
    _iterations = atoi(_args[0].c_str()); _args.erase(_args.begin());
    CommandRunner::FactoryNoneReason reason;

    if (CommandRunner::collectCommandBlock(specStream, _subCommands, reason) != CommandRunner::FactoryNoneReason::FactoryNoneEnd)
        throw invalid_argument("Unterminated command block");
}

void CommandLoop::execute(void)
{
    if (CommandConditional::evaluateCondition(_args) || !_iterations)
        setEvent(Command::ExecEvents::EventExecutionEnded);
    else
    {
        --_iterations;
        CommandRunner::GetInstance()->executeSequence(_subCommands);
        setEvent(Command::ExecEvents::EventExecutionProgress);
    }
}
