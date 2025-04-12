// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "Actor.h"
#include "AIComponent.h"

class Enemy : public Actor
{
public:
	Enemy(class Game *game);
	~Enemy();
	void UpdateActor(float deltaTime) override;
	class CircleComponent *GetCircle() { return mCircle; }
	void ToDeath() { SetState(EDead); }
	void OnHitBullet();
	/**
	 * TowerがEnemyをターゲットできるかどうかを判定
	 * 「検知できるかどうか」なので、流れ弾には当たる
	 *
	 */
	bool IsAllowTargeting();
	void LockedOnLog(const char *str = "")
	{
		SDL_Log("Locked on to enemy %s: %s", mAI->GetCurrentStateName(), str);
	}

private:
	void InitializeAIStates();

	class CircleComponent *mCircle;
	class AIComponent *mAI;
	class NavComponent *mNav;

	void StopMove();
	void StartMove();

	float mForwardSpeed = 150.0f;
	bool mIsResurrected = false;

	void HandleEnterBeingRepaired();
	void HandleEnterInvestigate();
};
