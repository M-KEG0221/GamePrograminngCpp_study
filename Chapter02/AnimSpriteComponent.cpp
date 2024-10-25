// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "AnimSpriteComponent.h"
#include "Math.h"
#include "Actor.h"
#include <iterator>

AnimSpriteComponent::AnimSpriteComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mCurrFrame(0.0f)
	, mAnimFPS(24.0f)
	, mOldAnimState(0)
{
}

void AnimSpriteComponent::Update(float deltaTime)
{
	SpriteComponent::Update(deltaTime);

	if (mAnimTextures.size() > 0)
	{
		// Update the current frame based on frame rate
		// and delta time
		mCurrFrame += mAnimFPS * deltaTime;

		//課題2 
		int currAnimState = mOwner->GetCurrAnimState();
		Animation currentAnim = mAnimations[currAnimState];

		//animが切り替わった場合、描画するtextureをリセット
		if (mOldAnimState != currAnimState)
		{
			mCurrFrame = static_cast<float>(currentAnim.first);
		}

		//ループしないアニメーションの場合、ラップで調整されるされる前に最後のframeを描画するようにする
		if (!currentAnim.isLoop && mCurrFrame > currentAnim.last)
		{
			mCurrFrame = static_cast<float>(currentAnim.last);
		}

		// Wrap current frame if needed
		//課題2
		// 『 > currentAnim.last』だと、キャスト時に小数点切り捨てとなり、1つ手前のアニメーションがチラチラ描画される
		while (mCurrFrame >= currentAnim.last + 1)
		{
			//課題2 描画対象のアニメーションの枚数分減算するようにする
			mCurrFrame -= currentAnim.CalcTotalFrames();
		}

		// Set the current texture
		SetTexture(mAnimTextures[static_cast<int>(mCurrFrame)]);

		mOldAnimState = currAnimState;
	}
}

void AnimSpriteComponent::SetAnimTextures(const std::vector<SDL_Texture*>& textures)
{
	mAnimTextures = textures;
	if (mAnimTextures.size() > 0)
	{
		// Set the active texture to first frame
		mCurrFrame = 0.0f;
		SetTexture(mAnimTextures[0]);
	}
}

//課題2
//デフォルト引数はhファイルにのみ書く
//こっちにも書くと：コンパイルエラー（ https://qiita.com/yut-nagase/items/29d0fc0984e6dbace85e ）
//こっちにのみ書くと：構文エラー
void AnimSpriteComponent::RegisterAnimation(const int animStateKey, const std::vector<SDL_Texture*>& textures, const bool isLoop)
{
	if (textures.size() == 0)
	{
		SDL_Log("空のアニメーションが登録されかけました。");
		return;
	}
	mAnimations[animStateKey] = Animation(
		mAnimTextures.size(),
		mAnimTextures.size() + textures.size() - 1,
		isLoop
	);

	std::vector<SDL_Texture*> result = mAnimTextures;
	result.reserve(mAnimTextures.size() + textures.size()); // 事前に追加分のメモリを確保することで効率化
	std::copy(textures.begin(), textures.end(), std::back_inserter(result));
	SetAnimTextures(result);
}