#pragma once
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include "Tetromino.h"
#include "TetrominoDataLoader.h"

// 0 = Room to move
// 1 = Active tetromino
// 2 = Locked tetromino
class Board
{
public:
	Board();
	~Board();

	std::vector<Tetromino> m_Tetrominos;

	int m_SpawnPostion[2] = { 0, 3 };
	std::unique_ptr<Tetromino> m_ActiveTetromino;

	bool UpdateBoard();
	bool SpawnTetromino();
	void DrawBoard();
	void LockTetromino();
	void ClearBoard();
	void PrintBoard();
	void HandleMovement(TetrominoMove dir);
	bool CanMove(TetrominoMove dir);
	void KickIntoBounds(Tetromino& tetromino);
	bool IntersectsWithSettled(Tetromino tetromino);

private:
	std::vector<std::vector<int>> m_Board;

};

