// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AIState.h"
#include "AIComponent.h"
#include "Actor.h"
#include "Game.h"
#include "Grid.h"
#include "Tile.h"
#include "Math.h"

void AIState::Update(float deltaTime)
{
	UpdateLog();

	HandleUpdate(deltaTime);
};

void AIState::OnEnter()
{
	EnterLog();

	HandleEnter();
	mCallBackEnter();
};

void AIState::OnExit()
{
	ExitLog();

	HandleExit();
	mCallBackExit();
};

void AIPatrol::HandleUpdate(float deltaTime)
{
	bool dead = true;
	if (dead)
	{
		mOwner->ChangeState(EAIState::Death);
	}
}
void AIPatrol::HandleEnter()
{
}

void AIPatrol::HandleExit()
{
}

void AIDeath::HandleUpdate(float deltaTime)
{
}

void AIDeath::HandleEnter()
{
	mOwner->GetOwner()->SetState(Actor::State::EDead);
}

void AIDeath::HandleExit()
{
}

void AIAttack::HandleUpdate(float deltaTime)
{
}

void AIAttack::HandleEnter()
{
}

void AIAttack::HandleExit()
{
}

void AIPatrol::HandleEnter()
{
}

void AIPatrol::HandleExit()
{
}

void AIDeath::HandleUpdate(float deltaTime)
{
}

void AIDeath::HandleEnter()
{
	mOwner->GetOwner()->SetState(Actor::State::EDead);
}

void AIDeath::HandleExit()
{
}

void AIAttack::HandleUpdate(float deltaTime)
{
}

void AIAttack::HandleEnter()
{
}

void AIAttack::HandleExit()
{
}

void AIPatrol::HandleEnter()
{
}

void AIPatrol::HandleExit()
{
}

void AIDeath::HandleUpdate(float deltaTime)
{
}

void AIDeath::HandleEnter()
{
	mOwner->GetOwner()->SetState(Actor::State::EDead);
}

void AIDeath::HandleExit()
{
}

void AIAttack::HandleUpdate(float deltaTime)
{
}

void AIAttack::HandleEnter()
{
}

void AIAttack::HandleExit()
{
}

void AIInvestigate::HandleUpdate(float deltaTime)
{
	Actor *ownerActor = mOwner->GetOwner();

	Vector2 diff = ownerActor->GetPosition() - ownerActor->GetGame()->GetGrid()->GetEndTile()->GetPosition();
	if (Math::NearZero(diff.Length(), 10.0f))
	{
		mOwner->ChangeState(EAIState::Death);
	}
}
void AIInvestigate::HandleEnter()
{
}
void AIInvestigate::HandleExit()
{
}

void AIBeingRepaired::HandleUpdate(float deltaTime)
{
	mRepairingTimer += deltaTime;

	if (mRepairingTimer >= M_REPAIRED_TIME)
	{
		mOwner->ChangeState(EAIState::Investigate);
		mRepairingTimer = 0.0f;
	}
}
void AIBeingRepaired::HandleEnter()
{
	/*
	TODO: owner‚ÌŒ©‚½–Ú‚ð”¼“§–¾‚É‚·‚é
	*/

	if (mRepairCount >= M_REPAIRABLE_COUNT)
	{
		mOwner->ChangeState(EAIState::Death);
	}

	mRepairCount++;
}
void AIBeingRepaired::HandleExit()
{
}
