#include "CommandRunner.h"
#include "CommandParallel.h"

CommandParallel::CommandParallel(vector<string> args, std::ifstream& specStream) : Command(args)
{
    CommandRunner::FactoryNoneReason reason;

    if (CommandRunner::collectCommandBlock(specStream, _subCommands, reason) != CommandRunner::FactoryNoneReason::FactoryNoneEnd)
        throw invalid_argument("Unterminated command block");
}

void CommandParallel::execute(void)
{
    CommandRunner::GetInstance()->executeSequence(_subCommands, false);
    setEvent(Command::ExecEvents::EventExecutionEnded);
}
