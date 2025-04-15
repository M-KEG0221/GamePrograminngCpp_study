// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include <vector>

struct GTNodeBoard;

class BoardState
{
public:
	enum SquareState
	{
		Empty,
		Red,
		Yellow
	};
	BoardState();
	std::vector<BoardState *> GetPossibleMoves(SquareState player) const;
	// std::vector<GTNodeBoard *> GetPossibleMoveNodes(SquareState player) const;
	std::vector<GTNodeBoard *> GetPossibleMoveNodes(SquareState player, int depth) const;
	/**
	 * ボードゲームが終了状態かどうかを判定する
	 * Terminal=終端の意味
	 * @return 終了状態ならtrue
	 */
	bool IsTerminal() const;
	float GetScore() const;

	SquareState mBoard[6][7];
	/**
	 * ミニマックス法に基づいて最善の手を決定する
	 * @param 基点となるルートノード（現在のボード状態）
	 * @return 最善の手を決定したノード
	 */
	const GTNodeBoard *MiniMaxDecide(const GTNodeBoard *node) const;
	BoardState *const AlphaBetaDecide(const BoardState* const root, const int maxDepth) const;
	/**
	 * Max側（プレイヤー）がアルファベータ法に基づいて最善の手を決定する
	 * @param 最善の手を選択した場合の評価値(float)
	 */
	const float AlphaBetaMax(const BoardState* const node, const int depth, float alpha, const float beta) const;
	/**
	 * Min側（CPU）がアルファベータ法に基づいて最善の手を決定する
	 * @param 最善の手を選択した場合の評価値(float)
	 */
	const float AlphaBetaMin(const BoardState* const node, const int depth, const float alpha, float beta) const;

protected:
	bool IsFull() const;
	int GetFourInARow() const;
	float CalculateHeuristic() const;

	const float FindMaxScorePlayer(const GTNodeBoard *node) const;
	const float FindMinScorePlayer(const GTNodeBoard *node) const;
};

// Try to place the player's piece
bool TryPlayerMove(class BoardState *state, int column);

// Make the next CPU move
void CPUMove(class BoardState *state);

struct GTNodeBoard
{
	std::vector<GTNodeBoard *> mChildren;
	class BoardState mScore;

	GTNodeBoard(std::vector<GTNodeBoard *> children, class BoardState score)
		: mChildren(children), mScore(score)
	{
	}
};