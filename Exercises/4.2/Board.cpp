// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
//
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "Board.h"
#include "Random.h"
#include "SDL/SDL.h"

BoardState::BoardState()
{
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			mBoard[i][j] = Empty;
		}
	}
}

std::vector<BoardState *> BoardState::GetPossibleMoves(SquareState player) const
{
	std::vector<BoardState *> retVal;

	// For each column, find if a move is possible
	for (int col = 0; col < 7; col++)
	{
		for (int row = 5; row >= 0; row--)
		{
			if (mBoard[row][col] == BoardState::Empty)
			{
				retVal.emplace_back(new BoardState(*this));
				retVal.back()->mBoard[row][col] = player;
				break;
			}
		}
	}

	return retVal;
}

// std::vector<GTNodeBoard *> BoardState::GetPossibleMoveNodes(SquareState player) const
std::vector<GTNodeBoard *> BoardState::GetPossibleMoveNodes(SquareState player, int depth = 5) const
{
	SDL_Log("PossibleMoveNodes: %d", depth);
	std::vector<GTNodeBoard *> retVal;
	if (depth <= 0)
		return retVal;

	// For each column, find if a move is possible
	for (int col = 0; col < 7; col++)
	{
		for (int row = 5; row >= 0; row--)
		{
			if (mBoard[row][col] == BoardState::Empty)
			{
				// BoardState *const possibleNode = new BoardState(*this);
				// possibleNode->mBoard[row][col] = player;

				// const SquareState nextPlayer = (player == BoardState::Red) ? BoardState::Yellow : BoardState::Red;

				// retVal.emplace_back(new GTNodeBoard(possibleNode->GetPossibleMoveNodes(nextPlayer), *possibleNode));

				BoardState possibleNode = BoardState(*this);
				possibleNode.mBoard[row][col] = player;

				const SquareState nextPlayer = (player == BoardState::Red) ? BoardState::Yellow : BoardState::Red;

				retVal.emplace_back(new GTNodeBoard(possibleNode.GetPossibleMoveNodes(nextPlayer, depth - 1), possibleNode));
				// TODO: これで本当に動くか検証したい
				break;
			}
		}
	}

	return retVal;
}

bool BoardState::IsTerminal() const
{
	// Is the board full?
	if (IsFull())
	{
		return true;
	}

	// Is there a four-in-a-row?
	int fourInRow = GetFourInARow();
	if (fourInRow != 0)
	{
		return true;
	}

	return false;
}

float BoardState::GetScore() const
{
	// If the board is full, the score is 0
	if (IsFull())
	{
		return 0.0f;
	}

	// Is there a four-in-a-row?
	int fourInRow = GetFourInARow();
	if (fourInRow != 0)
	{
		return static_cast<float>(fourInRow);
	}

	return CalculateHeuristic();
}

bool BoardState::IsFull() const
{
	bool isFull = true;
	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 7; j++)
		{
			if (mBoard[i][j] == Empty)
			{
				isFull = false;
			}
		}
	}

	return isFull;
}

int BoardState::GetFourInARow() const
{
	// Returns -1 if yellow wins, 1 if red wins, 0 otherwise

	// Check if there's a row with four in a row
	for (int row = 0; row < 6; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			if (mBoard[row][col] == mBoard[row][col + 1] &&
				mBoard[row][col] == mBoard[row][col + 2] &&
				mBoard[row][col] == mBoard[row][col + 3])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}

	// Check if there's a column with four in a row
	for (int col = 0; col < 7; col++)
	{
		for (int row = 0; row < 3; row++)
		{
			if (mBoard[row][col] == mBoard[row + 1][col] &&
				mBoard[row][col] == mBoard[row + 2][col] &&
				mBoard[row][col] == mBoard[row + 3][col])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}

	// Check if there's a right diagonal four in a row
	for (int col = 0; col < 4; col++)
	{
		for (int row = 0; row < 3; row++)
		{
			if (mBoard[row][col] == mBoard[row + 1][col + 1] &&
				mBoard[row][col] == mBoard[row + 2][col + 2] &&
				mBoard[row][col] == mBoard[row + 3][col + 3])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}

	// Check if there's a left diagonal for in a row
	for (int col = 0; col < 4; col++)
	{
		for (int row = 3; row < 6; row++)
		{
			if (mBoard[row][col] == mBoard[row - 1][col + 1] &&
				mBoard[row][col] == mBoard[row - 2][col + 2] &&
				mBoard[row][col] == mBoard[row - 3][col + 3])
			{
				if (mBoard[row][col] == BoardState::Yellow)
				{
					return -1;
				}
				else if (mBoard[row][col] == BoardState::Red)
				{
					return 1;
				}
			}
		}
	}
	return 0;
}

float BoardState::CalculateHeuristic() const
{
	// TODO: You could change this to calculate an actual heuristic
	return 0.0f;
}

bool TryPlayerMove(BoardState *state, int column)
{
	// Find the first row in that column that's available
	// (if any)
	for (int row = 5; row >= 0; row--)
	{
		if (state->mBoard[row][column] == BoardState::Empty)
		{
			state->mBoard[row][column] = BoardState::Yellow;
			return true;
		}
	}

	return false;
}

void CPUMove(BoardState *state)
{
	/*
	ここが変更対象
	 */

	// For now, this just randomly picks one of the possible moves
	std::vector<BoardState *> moves = state->GetPossibleMoves(BoardState::Red);
	std::vector<GTNodeBoard *> moveNodes = state->GetPossibleMoveNodes(BoardState::Red);
	const GTNodeBoard *rootNode = new GTNodeBoard(moveNodes, *state);
	// BoardState bestMove = state->MiniMaxDecide(rootNode)->mScore;
	BoardState *bestMove = state->AlphaBetaDecide(state, 5);

	int index = Random::GetIntRange(0, moves.size() - 1);

	// *state = *moves[index];
	*state = *bestMove; // stateのアドレスではなｋ、stateのアドレス先(=gameが持つmBoardState)を上書きする

	// Clear up memory from possible moves
	for (auto state : moves)
	{
		delete state;
	}
	for (auto nodes : moveNodes)
	{
		delete nodes;
	}
	// delete rootNode;
}

/*
1. 単純なミニマックス法で実装する（component化せずにBoardに実装する）
	- 4.3.1 [x]
		- ゲーム木を生成して、実際に動かす
	- 4.3.2 []
2. アルファベータ法で実装する
3. component化してみる
*/

const GTNodeBoard *BoardState::MiniMaxDecide(const GTNodeBoard *node) const
{
	const GTNodeBoard *bestChild = nullptr;
	float maxValue = -std::numeric_limits<float>::infinity();

	for (const GTNodeBoard *child : node->mChildren)
	{
		float value = FindMinScorePlayer(child);
		if (value > maxValue)
		{
			maxValue = value;
			bestChild = child;
		}
	}

	return bestChild;
}

const float BoardState::FindMaxScorePlayer(const GTNodeBoard *node) const
{
	// Check if the node is terminal
	if (node->mScore.IsTerminal() || node->mChildren.empty()) // IsTerminalが必要かは不明（AIが予測表示したのを単に受けいれただけのため）
	{
		return node->mScore.GetScore();
	}

	float maxValue = -std::numeric_limits<float>::infinity();
	for (const GTNodeBoard *child : node->mChildren)
	{
		maxValue = std::max(maxValue, FindMinScorePlayer(child));
	}

	return maxValue;
}

const float BoardState::FindMinScorePlayer(const GTNodeBoard *node) const
{
	// Check if the node is terminal
	if (node->mScore.IsTerminal() || node->mChildren.empty()) // IsTerminalが必要かは不明（AIが予測表示したのを単に受けいれただけのため）
	{
		return node->mScore.GetScore();
	}

	float minValue = std::numeric_limits<float>::infinity();
	for (const GTNodeBoard *child : node->mChildren)
	{
		minValue = std::min(minValue, FindMaxScorePlayer(child));
	}

	return minValue;
}

BoardState *const BoardState::AlphaBetaDecide(const BoardState *const root, const int maxDepth) const
{
	BoardState *choice = nullptr;
	float alpha = -std::numeric_limits<float>::infinity();
	float beta = std::numeric_limits<float>::infinity();
	for (BoardState *const child : root->GetPossibleMoves(BoardState::Red))
	{
		float value = AlphaBetaMin(child, maxDepth - 1, alpha, beta);
		if (value > alpha)
		{
			alpha = value;
			choice = child;
		}
	}

	return choice;
}

const float BoardState::AlphaBetaMax(const BoardState *const node, const int depth, float alpha, const float beta) const
{
	// Check if the node is terminal
	if (node->IsTerminal() || depth == 0)
	{
		return node->GetScore();
	}

	float maxValue = -std::numeric_limits<float>::infinity();
	for (const BoardState *const child : node->GetPossibleMoves(BoardState::Red))
	{
		maxValue = std::max(maxValue, AlphaBetaMin(child, depth - 1, alpha, beta));
		if (maxValue >= beta)
		{
			return maxValue;
		}
		alpha = std::max(alpha, maxValue);
	}

	return maxValue;
}

const float BoardState::AlphaBetaMin(const BoardState *const node, const int depth, const float alpha, float beta) const
{
	// Check if the node is terminal
	if (node->IsTerminal() || depth == 0)
	{
		return node->GetScore();
	}

	float minValue = std::numeric_limits<float>::infinity();
	for (const BoardState *const child : node->GetPossibleMoves(BoardState::Yellow))
	{
		minValue = std::min(minValue, AlphaBetaMax(child, depth - 1, alpha, beta));
		if (minValue <= alpha)
		{
			return minValue;
		}
		beta = std::min(beta, minValue);
	}

	return minValue;
}