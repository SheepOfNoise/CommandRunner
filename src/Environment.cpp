#include "Environment.h"

Environment* Environment::_instance = NULL;

void Environment::setVarTarget(StateVariables var, int target)
{
    stState& varState = _states[static_cast<int>(var)];
  
    varState.targetValue = target;
    if (var == StateVariables::StateX)
        varState.delta = target - varState.currentValue < 180 ? 1 : -1;
    else
        varState.delta = target > 0 ? 1 : -1;
};

Command::ExecEvents Environment::WaitForEvent(StateVariables varType)
{
    stState& varState = _states[static_cast<int>(varType)];
    Command::ExecEvents event = Command::ExecEvents::EventExecutionEnded;

    if (varState.inMotion)
    {
        if (!varState.delta || abs(varState.targetValue - varState.currentValue) <= abs(varState.delta))
        {
            varState.inMotion = false;
            _logger->debug("Motion stopped for {}", static_cast<int>(varType));
            if (varType == StateVariables::StateY || varType == StateVariables::StateZ)
                varState.currentValue = 0;
        }
        else
        {
            event = Command::ExecEvents::EventExecutionProgress;
            varState.currentValue += varState.delta;
            if (varType == StateVariables::StateX)
                varState.currentValue = (varState.currentValue + 360) % 360;
            else if (varType == StateVariables::StateZ)
            {
                double angle = _states[static_cast<int>(StateVariables::StateX)].currentValue * DEG_TO_RAD;
                _locationX += varState.delta * sin(angle);
                _locationY += varState.delta * cos(angle);
            }
            _logger->debug("Variable {} at {}", static_cast<int>(varType), varState.currentValue);
        }
    }
    else if (varState.delta)
    {
        varState.inMotion = true;
        event = Command::ExecEvents::EventExecutionStarted;
        _logger->debug("Motion started for {}", static_cast<int>(varType));
    }

    return event;
}
