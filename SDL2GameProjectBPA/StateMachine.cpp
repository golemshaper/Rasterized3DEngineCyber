#include "StateMachine.h"

void StateMachine::MapState(int StateID, std::function<void()> OnUpdate)
{
	TickMap[StateID] = OnUpdate;
}

void StateMachine::MapState(int StateID, std::function<void()> OnUpdate, std::function<void()> OnEnter)
{
	TickMap[StateID] = OnUpdate;
	EnterMap[StateID] = OnEnter;
}

void StateMachine::MapState(int StateID, std::function<void()> OnUpdate, std::function<void()> OnEnter, std::function<void()> OnExit)
{
	TickMap[StateID] = OnUpdate;
	EnterMap[StateID] = OnEnter;
	ExitMap[StateID] = OnExit;
}

void StateMachine::SetState(int NState)
{
	if (NState == CurrentState)
	{
		return;
	}
	if (ExitMap.find(CurrentState) != ExitMap.end()) {
		ExitMap[CurrentState](); //OnExitCalled
	}
	CurrentState = NState;
	if (EnterMap.find(CurrentState) != EnterMap.end()) {
		EnterMap[CurrentState](); //OnExitCalled
	}
}

void StateMachine::Tick(float DeltaTime)
{
	if (CurrentState ==-1)return;
	TimeInState += DeltaTime;
	StateDeltaTime = DeltaTime;
	if (TickMap.find(CurrentState) != TickMap.end()) {
		TickMap[CurrentState]();
	}
	
}
