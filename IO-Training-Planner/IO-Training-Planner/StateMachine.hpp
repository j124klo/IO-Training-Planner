#pragma once

#include <memory>
#include <stack>

#include "State.hpp"

using namespace std;

typedef std::unique_ptr<State> StateRef;

class StateMachine
{
private:
	stack<StateRef> _states;
	StateRef _newState;

	bool _isRemoving;
	bool _isAdding;
	bool _isReplacing;

public:
	StateMachine() : _isRemoving(false), _isAdding(false), _isReplacing(false) {}
	~StateMachine() {}

	void AddState(StateRef newState, bool isReplacing = true);
	void RemoveState();

	void ProcessStateChanges();

	StateRef& GetActiveState();
};