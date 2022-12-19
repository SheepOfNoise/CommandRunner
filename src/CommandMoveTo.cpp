#include "CommandRunner.h"
#include "CommandMoveTo.h"
#include "CommandTurn.h"
#include "CommandMove.h"

double calcDotProduct(double v1x, double v1y, double v2x, double v2y) {
    return v1x * v2x + v1y * v2y;
}

double calcCrossProduct(double v1x, double v1y, double v2x, double v2y) {
    return v1x * v2y - v1y * v2x;
}

bool directionCW(double targetX, double targetY) {
    double currentDir =
        Environment::GetInstance()->getCurrentValue(Environment::StateVariables::StateX) * DEG_TO_RAD;

    return
        calcCrossProduct(sin(currentDir), cos(currentDir), targetX, targetY) < 0;
}

double calcTurnAngleDelta(double targetX, double targetY)
{
    double currentDir =
        Environment::GetInstance()->getCurrentValue(Environment::StateVariables::StateX) * DEG_TO_RAD;

    return -atan2(
        calcCrossProduct(cos(currentDir), sin(currentDir), targetY, targetX),
    calcDotProduct(cos(currentDir), sin(currentDir), targetY, targetX)) / DEG_TO_RAD;
}

double CommandMoveTo::calcDistance(void) {
    double currentX = Environment::GetInstance()->getLocation('X');
    double currentY = Environment::GetInstance()->getLocation('Y');

    return sqrt((_targetY - currentY) * (_targetY - currentY) + (_targetX - currentX) * (_targetX - currentX));
}

void CommandMoveTo::preExecute(void) {
    if (!_preExecuted)
    {
        double currentX = Environment::GetInstance()->getLocation('X');
        double currentY = Environment::GetInstance()->getLocation('Y');
        double targetDir = atan2(_targetX - currentX, _targetY - currentY);
        int deltaDirection = -(int)calcTurnAngleDelta(sin(targetDir), cos(targetDir));
        string turnDirection = directionCW(cos(targetDir), sin(targetDir)) ? "right" : "left";
        _legCommands.push_back(new CommandTurn(vector<string>{turnDirection, to_string(deltaDirection)}));
        _legCommands.push_back(new CommandMove(vector<string>{to_string(calcDistance())}));
    }
    _preExecuted = true;
}

void CommandMoveTo::execute(void)
{
    CommandRunner::GetInstance()->executeSequence(_legCommands);
    setEvent(Command::ExecEvents::EventExecutionEnded);
}