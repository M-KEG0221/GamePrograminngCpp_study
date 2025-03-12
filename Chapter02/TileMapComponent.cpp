#include "TileMapComponent.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

TileMapComponent::TileMapComponent(Actor* owner, int drawOrder)
	:SpriteComponent(owner, drawOrder)
	, mTileSet()
{
}

void TileMapComponent::SetTileSet(SDL_Texture* tileSetTexture, const int rows, const int columns)
{
	mTileSet.mTexture = tileSetTexture;
	mTileSet.columns = columns;
	mTileSet.rows = rows;
	SDL_QueryTexture(tileSetTexture, NULL, NULL, &mTileSet.width, &mTileSet.height);
}

void TileMapComponent::Update(float deltaTime)
{
	//もしスクロールに対応させたいなら
}

void TileMapComponent::Draw(SDL_Renderer* renderer)
{
	SDL_Rect srcRect;
	//タイルは全て同じ大きさなので予め設定
	srcRect.w = static_cast<int>(mTileSet.width / mTileSet.columns);
	srcRect.h = static_cast<int>(mTileSet.height / mTileSet.rows);

	int rowIdx = 0;
	for (auto& tileMapRow : mTileMap)
	{
		int columnIdx = 0;
		for (auto& tile : tileMapRow)
		{
			if (tile != -1)
			{
				// 描画したいタイルマップ上のタイル位置
				srcRect.x = static_cast<int>((tile % mTileSet.columns) * srcRect.w);
				srcRect.y = static_cast<int>((tile / mTileSet.columns) * srcRect.h);

				SDL_Rect dstRect;
				// タイルの描画サイズ
				dstRect.w = static_cast<int>(mScreenSize.x / tileMapRow.size());
				dstRect.h = static_cast<int>(mScreenSize.y / mTileMap.size());
				// タイルの描画位置
				dstRect.x = static_cast<int>(columnIdx * dstRect.w);
				dstRect.y = static_cast<int>(rowIdx * dstRect.h);

				SDL_RenderCopy(renderer,
					mTileSet.mTexture,
					&srcRect,
					&dstRect
				);
			}
			columnIdx++;
		}
		rowIdx++;
	}
}

void TileMapComponent::LoadCSV(const std::string& fileName)
{
	//csvファイル読み込み
	std::ifstream tileMap(fileName);//「tileMap」は変数名

	//ファイルが見つからなかった場合のエラー処理
	if (!tileMap)
	{
		SDL_Log("TileMapComponent：csvファイルが見つかりませんでした %s", fileName.c_str());
		return;//要検討。ゲームを落とす処理がベスト？
	}

	//csvファイル1行をtilesRowStrに入れる。それを最後の行まで繰り返す
	std::string tilesRowStr;
	while (std::getline(tileMap, tilesRowStr))
	{
		std::vector<int> addTilesRow;
		std::string tileStr;
		std::istringstream iss(tilesRowStr);//入力専用の文字列ストリーム

		while (std::getline(iss, tileStr, ','))
		{
			addTilesRow.push_back(std::stoi(tileStr));//String TO Int );
		}

		mTileMap.push_back(addTilesRow);
	}
}