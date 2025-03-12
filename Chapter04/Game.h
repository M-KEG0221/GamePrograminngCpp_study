// ----------------------------------------------------------------
// 「C++によるゲームプログラミング」by Sanjay Madhav より
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// BSDライセンスのもとでリリース
// 詳細はルートディレクトリのLICENSEを参照
// ----------------------------------------------------------------

#pragma once
#include "SDL/SDL.h"
#include <unordered_map>
#include <string>
#include <vector>
#include "Math.h"

class Game
{
public:
	Game();
	bool Initialize();
	void RunLoop();
	void Shutdown();

	void AddActor(class Actor *actor);
	void RemoveActor(class Actor *actor);

	void AddSprite(class SpriteComponent *sprite);
	void RemoveSprite(class SpriteComponent *sprite);

	SDL_Texture *GetTexture(const std::string &fileName);

	class Grid *GetGrid() { return mGrid; }
	std::vector<class Enemy *> &GetEnemies() { return mEnemies; }
	class Enemy *GetNearestEnemy(const Vector2 &pos);

private:
	void ProcessInput();
	void UpdateGame();
	void GenerateOutput();
	void LoadData();
	void UnloadData();

	// ロードされたテクスチャのマップ
	std::unordered_map<std::string, SDL_Texture *> mTextures;

	// ゲーム内のすべてのアクター
	std::vector<class Actor *> mActors;
	// 保留中のアクター
	std::vector<class Actor *> mPendingActors;

	// 描画されるすべてのスプライトコンポーネント
	std::vector<class SpriteComponent *> mSprites;

	SDL_Window *mWindow;
	SDL_Renderer *mRenderer;
	Uint32 mTicksCount;
	bool mIsRunning;
	// 現在アクターを更新中かどうかを追跡
	bool mUpdatingActors;

	// ゲーム固有
	std::vector<class Enemy *> mEnemies;
	class Grid *mGrid;
	float mNextEnemy;
};
