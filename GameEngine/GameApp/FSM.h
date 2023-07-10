#pragma once

class BaseState;

class FSM
{
private:
	BaseState* curState;
public:
	FSM(BaseState* initState)
	{
		curState = initState;
		ChangeState(curState);
	}

	void ChangeState(BaseState* nextState)
	{
		if (nextState == curState)
		{
			return;
		}

		if (curState != nullptr)
		{
			curState->OnStateExit();
		}
		curState = nextState;
		curState->OnStateEnter();
	}

	void Update()
	{
		if (curState != nullptr)
		{
			curState->OnStateUpdate();
		}
	}
};

