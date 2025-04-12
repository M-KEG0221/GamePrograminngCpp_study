// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AIComponent.h"
#include "Actor.h"
#include "AIState.h"
#include <SDL/SDL_log.h>

AIComponent::AIComponent(class Actor *owner)
	: Component(owner), mCurrentState(nullptr)
{
}

void AIComponent::Update(float deltaTime)
{
	if (mCurrentState)
	{
		mCurrentState->Update(deltaTime);
	}
}

void AIComponent::ChangeState(AIState::EAIState stateName)
{
	// First exit the current state
	if (mCurrentState)
	{
		mCurrentState->OnExit();
	}

	mCurrentState = FindState(stateName);
	mCurrentState->OnEnter();
}

void AIComponent::RegisterState(AIState *state)
{
	if (FindState(state->GetEnum()) == nullptr)
	{
		// Register the state
		mStateMap.emplace(state->GetName(), state);
		return;
	}

	// ERROR: State already registered
	SDL_Log("AIState %s already registered", state->GetName());
}

AIState *AIComponent::FindState(AIState::EAIState state)
{
	auto iter = mStateMap.find(state);
	if (iter != mStateMap.end())
	{
		return iter->second;
	}
	else
	{
		SDL_Log("Could not find AIState %s in state map", AIState::GetStateName(state));
		return nullptr;
	}
}