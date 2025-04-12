// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Component.h"
#include <unordered_map>
#include <string>
#include "AIState.h"

class AIComponent : public Component
{
public:
	AIComponent(class Actor *owner);

	void Update(float deltaTime) override;
	// void ChangeState(const std::string &name);
	void ChangeState(AIState::EAIState stateName);

	// Add a new state to the map
	void RegisterState(class AIState *state);

	const char *GetCurrentStateName() const { return mCurrentState->GetName(); }
	const AIState::EAIState GetCurrentStateEnum() const { return mCurrentState->GetEnum(); }

	class Actor *GetOwner() const { return mOwner; }

private:
	// Maps name of state to AIState instance
	std::unordered_map<AIState::EAIState, class AIState *> mStateMap;
	// Current state we're in
	class AIState *mCurrentState;

	class AIState *FindState(AIState::EAIState stateName);
};
