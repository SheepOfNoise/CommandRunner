#include "CommandConditional.h"
#include "CommandRunner.h"
#include "Environment.h"

std::string trim(const std::string& str,
    const std::string& whitespace = " \t")
{
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

CommandConditional::CommandConditional(vector<string> args, std::ifstream& specStream) : Command(args) {
    _logger = CLogger::GetInstance()->CreateLogger(typeid(*this).name());
    std::string line;
    vector<Command*>* currentClauseCommands = &_commandsWhenTrue;
    do {
        CommandRunner::FactoryNoneReason reason;
        Command* command = CommandRunner::GetInstance()->CommandFactory(specStream, reason);
        switch (reason)
        {
        case CommandRunner::FactoryNoneReason::FactorySuccess:
            currentClauseCommands->push_back(command);
            break;
        case CommandRunner::FactoryNoneReason::FactoryNoneElse:
            currentClauseCommands = &_commandsWhenFalse;
            break;
        case CommandRunner::FactoryNoneReason::FactoryNoneEOF:
        case CommandRunner::FactoryNoneReason::FactoryNoneEnd:
            return;
        }
    } while (true);
}

static double evaluateNumber(string operand)
{
    char* end;
    double result = strtod(operand.c_str(), &end);

    if (end == operand || *end != '\0')
        throw invalid_argument(string("Invalid number") + operand);

    return result;
}

static double evaluateOperand(string operand)
{
    return
        "x" == operand ? Environment::GetInstance()->getCurrentValue(Environment::StateVariables::StateX) :
        "y" == operand ? Environment::GetInstance()->getCurrentValue(Environment::StateVariables::StateY) :
        "z" == operand ? Environment::GetInstance()->getCurrentValue(Environment::StateVariables::StateZ) :
        "fork-sensor" == operand ? Environment::GetInstance()->binOnFork() :
        "locationX" == operand ? Environment::GetInstance()->getLocation('X') :
        "locationY" == operand ? Environment::GetInstance()->getLocation('Y') :
        "true" == operand ? 1.0 :
        "false" == operand ? 0.0 :
        evaluateNumber(operand);
}

bool CommandConditional::evaluateCondition(vector<string> args)
{
    double operand1 = evaluateOperand(args[0]);
    string operation = args[1];
    double operand2 = evaluateOperand(args[2]);
    bool result =
        "eq" == operation ? operand1 == operand2 :
        ">" == operation ? operand1 > operand2 :
        ">=" == operation ? operand1 >= operand2 :
        "<" == operation ? operand1 < operand2 :
        operand1 <= operand2;

    return result;
}

void CommandConditional::execute(void)
{
    CommandRunner::GetInstance()->executeSequence(_conditionTrue ? _commandsWhenTrue : _commandsWhenFalse);

    setEvent(Command::ExecEvents::EventExecutionEnded);
}
