#include "CommandTurn.h"

void CommandTurn::execute(void)
{
    Command::ExecEvents event;
    
    handleEvent(event = Environment::GetInstance()->WaitForEvent(Environment::StateVariables::StateX));

    setEvent( event);
}
