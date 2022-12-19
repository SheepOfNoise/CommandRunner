#include "CommandMove.h"

void CommandMove::execute(void)
{
    Command::ExecEvents event;
    
    handleEvent(event = Environment::GetInstance()->WaitForEvent(Environment::StateVariables::StateZ));
    
    setEvent( event);
}
