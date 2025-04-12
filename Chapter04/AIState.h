// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once

#include <functional>
#include <SDL/SDL_log.h>

class AIState
{
public:
	enum class EAIState
	{
		Patrol,
		Death,
		Attack,
		Investigate,
		BeingRepaired
	};

	using CallBackFunc = std::function<void(void)>;

	AIState(class AIComponent *owner) : mOwner(owner)
	{
	}

	void SetCallBackEnter(const CallBackFunc func) { mCallBackEnter = func; }
	void SetCallBackExit(CallBackFunc func) { mCallBackExit = func; }

	// State-specific behavior
	void Update(float deltaTime);
	void OnEnter();
	void OnExit();

	// Getter for string name of state
	virtual const char *GetName() const = 0;
	virtual const EAIState GetEnum() const = 0;

	static const char *GetStateName(EAIState state)
	{
		switch (state)
		{
		case EAIState::Patrol:
			return AIPatrol(nullptr).GetName();
		case EAIState::Death:
			return AIDeath(nullptr).GetName();
		case EAIState::Attack:
			return AIAttack(nullptr).GetName();
		case EAIState::Investigate:
			return AIInvestigate(nullptr).GetName();
		case EAIState::BeingRepaired:
			return AIBeingRepaired(nullptr, 0.0f).GetName();
		default:
			return "Unknown State";
		}
	}

protected:
	class AIComponent *mOwner;

	// 具体処理群。継承クラスが実装する。

	virtual void HandleUpdate(float deltaTime) = 0;
	virtual void HandleEnter() = 0;
	virtual void HandleExit() = 0;

	CallBackFunc mCallBackEnter = []() {};
	CallBackFunc mCallBackExit = []() {};
	// 具体処理は各Stateの中に閉じさせたい（カスタマイズされたくない）ため、mCallBackUpdateは用意していない

private:
	void AIStateLog(const char *str)
	{
		SDL_Log("%s %s state", str, GetName());
	}

	void EnterLog()
	{
		AIStateLog("Entering");
	}

	void UpdateLog()
	{
		AIStateLog("Updating");
	}

	void ExitLog()
	{
		AIStateLog("Exiting");
	}
};

class AIPatrol : public AIState
{
public:
	AIPatrol(class AIComponent *owner)
		: AIState(owner)
	{
	}

	const char *GetName() const override
	{
		return "Patrol";
	}

	const EAIState GetEnum() const override
	{
		return EAIState::Patrol;
	}

private:
	// Override with behaviors for this state
	void HandleUpdate(float deltaTime) override;
	void HandleEnter() override;
	void HandleExit() override;
};

class AIDeath : public AIState
{
public:
	AIDeath(class AIComponent *owner)
		: AIState(owner)
	{
	}

	void HandleUpdate(float deltaTime) override;
	void HandleEnter() override;
	void HandleExit() override;

	const char *GetName() const override
	{
		return "Death";
	}

	const EAIState GetEnum() const override
	{
		return EAIState::Death;
	}
};

class AIAttack : public AIState
{
public:
	AIAttack(class AIComponent *owner)
		: AIState(owner)
	{
	}

	void HandleUpdate(float deltaTime) override;
	void HandleEnter() override;
	void HandleExit() override;

	const char *GetName() const override
	{
		return "Attack";
	}

	const EAIState GetEnum() const override
	{
		return EAIState::Attack;
	}
};

/**
 * エネミー用の探索状態（ゴールに向かって移動している状態）
 */
class AIInvestigate : public AIState
{
public:
	AIInvestigate(class AIComponent *owner)
		: AIState(owner) // TODO* 要修正かも？
	{
	}

	void HandleUpdate(float deltaTime) override;
	void HandleEnter() override;
	void HandleExit() override;

	const char *GetName() const override
	{
		return "Investigate";
	}

	const EAIState GetEnum() const override
	{
		return EAIState::Investigate;
	}
};
/**
 * エネミー用の修復状態（しばらくするとInvestigate状態で復活する）
 */
class AIBeingRepaired : public AIState
{
public:
	AIBeingRepaired(class AIComponent *owner, float repairedTime, int repairableCount = 1)
		: AIState(owner),
		  M_REPAIRED_TIME(repairedTime),
		  M_REPAIRABLE_COUNT(repairableCount)
	{
	}

	void HandleUpdate(float deltaTime) override;
	void HandleEnter() override;
	void HandleExit() override;

	const char *GetName() const override
	{
		return "BeingRepaired";
	}

	const EAIState GetEnum() const override
	{
		return EAIState::BeingRepaired;
	}

private:
	float mRepairingTimer = 0.0f;
	const float M_REPAIRED_TIME;

	int mRepairCount = 0;
	const int M_REPAIRABLE_COUNT;
};
