// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "MoveComponent.h"
#include "Actor.h"
#include <iostream>

MoveComponent::MoveComponent(class Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
	, mAngularSpeed(0.0f)
	, mForwardSpeed(0.0f)
	, mForce(Vector2())
	, mFriction(0.0f)
{

}

void MoveComponent::Update(float deltaTime)
{
	if (!Math::NearZero(mAngularSpeed))
	{
		float rot = mOwner->GetRotation();
		rot += mAngularSpeed * deltaTime;
		mOwner->SetRotation(rot);
	}

	//if (!Math::NearZero(mForwardSpeed)) // 物理計算は常に行われるため、条件を外す

	// F=m.aからa=F/mをし、加速度を求める（空気抵抗も考慮）
	Vector2 acceleration = (mForce - mVerocity * mFriction) / mMass;

	// v=v0+atから現在の速度にa*deltatimeを加算
	mVerocity += acceleration * deltaTime;

	//x=x0+vtから現在の位置にv*deltatimeを加算
	Vector2 pos = mOwner->GetPosition();
	pos += mVerocity * deltaTime;//deltaTimeが2回掛かるため、極端に遅くなる
	//x=v0t+at^2/2と厳密にしなくてもよい

	// (Screen wrapping code only for asteroids)
	if (pos.x < 0.0f) { pos.x = 1022.0f; }
	else if (pos.x > 1024.0f) { pos.x = 2.0f; }

	if (pos.y < 0.0f) { pos.y = 766.0f; }
	else if (pos.y > 768.0f) { pos.y = 2.0f; }

	mOwner->SetPosition(pos);

	//所持撃力をリセット
	mForce = Vector2();
}

void MoveComponent::AddForce(Vector2 force)
{
	mForce += force;
}

void MoveComponent::AddForce(float force)
{
	AddForce(force * mOwner->GetForward());
}

void MoveComponent::AddForce()
{
	AddForce(mForwardSpeed * 60);
}
