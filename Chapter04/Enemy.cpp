// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Enemy.h"
#include "Game.h"
#include "SpriteComponent.h"
#include "NavComponent.h"
#include "Grid.h"
#include "Tile.h"
#include "CircleComponent.h"
#include <algorithm>
#include "AIState.h"
#include "AIComponent.h"

using EAIState = AIState::EAIState;

Enemy::Enemy(class Game *game)
	: Actor(game)
{
	// Add to enemy vector
	game->GetEnemies().emplace_back(this);

	SpriteComponent *sc = new SpriteComponent(this);
	sc->SetTexture(game->GetTexture("Assets/Airplane.png"));
	// Set position at start tile
	SetPosition(GetGame()->GetGrid()->GetStartTile()->GetPosition());
	// Setup a nav component at the start tile
	mNav = new NavComponent(this);
	mForwardSpeed = 150.0f;
	mNav->SetForwardSpeed(mForwardSpeed);
	mNav->StartPath(GetGame()->GetGrid()->GetStartTile());
	// Setup a circle for collision
	mCircle = new CircleComponent(this);
	mCircle->SetRadius(25.0f);

	InitializeAIStates();
}

void Enemy::InitializeAIStates()
{
	/*
	cursorの自動推論が、コンストラクタにあったコードを以下のように完全に推測できていた
	すげぇ。
	*/

	mAI = new AIComponent(this);

	auto *investigate = new AIInvestigate(mAI);
	investigate->SetCallBackEnter([this]()
								  { HandleEnterInvestigate(); });
	mAI->RegisterState(investigate);

	auto *beingRepaired = new AIBeingRepaired(mAI, 5.0f);
	beingRepaired->SetCallBackEnter([this]()
									{ HandleEnterBeingRepaired(); });
	mAI->RegisterState(beingRepaired);

	auto *death = new AIDeath(mAI);
	mAI->RegisterState(death);

	mAI->ChangeState(EAIState::Investigate);
}

Enemy::~Enemy()
{
	// Remove from enemy vector
	auto iter = std::find(GetGame()->GetEnemies().begin(),
						  GetGame()->GetEnemies().end(),
						  this);
	GetGame()->GetEnemies().erase(iter);
}

void Enemy::UpdateActor(float deltaTime)
{
	Actor::UpdateActor(deltaTime);

	mAI->Update(deltaTime);
}

void Enemy::OnHitBullet()
{
	mAI->ChangeState(EAIState::BeingRepaired);
}

bool Enemy::IsAllowTargeting()
{
	return mAI->GetCurrentStateEnum() != AIState::EAIState::BeingRepaired;
}

void Enemy::StopMove()
{
	mNav->SetForwardSpeed(0.0f);
}

void Enemy::StartMove()
{
	mNav->SetForwardSpeed(mForwardSpeed);
}

void Enemy::HandleEnterBeingRepaired()
{
	StopMove();
	mIsResurrected = true;
};

void Enemy::HandleEnterInvestigate()
{
	StartMove();
};