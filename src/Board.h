#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <random>
#include "Tetromino.h"
#include "TetrominoDataLoader.h"
#include "SpriteRenderer.h"

struct Cell {
	int value = 0;
	glm::vec3 color{ 1.0f };
	std::shared_ptr<Texture> texture;
};

struct Rect {
	float x, y, w, h;
};

// 0 = Room to move
// 1 = Active tetromino
// 2 = Locked tetromino
class Board
{
public:
	Board();
	~Board();

	std::vector<Tetromino> m_Tetrominos;
	std::unique_ptr<Tetromino> m_ActiveTetromino;

	int m_SpawnPostion[2] = { 0, 3 };
	int m_BoardScore = 0;
	const std::map<int, int> m_Scores {
		{ 1, 100 },
		{ 2, 300 },
		{ 3, 500 },
		{ 4, 800 }
	};

	bool UpdateBoard();
	void DrawBoard();
	void ClearBoard();

	bool SpawnTetromino();
	void LockTetromino();
	bool HandleMovement(TetrominoMove dir);
	bool CanMove(TetrominoMove dir);
	void KickIntoBounds(Tetromino& tetromino);
	bool IntersectsWithSettled(Tetromino tetromino);

	void CheckLines();
	void ShiftBoardDown(int row);
	void UpdateScore(int lines);

	void Draw(SpriteRenderer& renderer, const Rect& board);

	std::unique_ptr<Texture> m_BoardBg;

private:
	std::vector<std::vector<Cell>> m_Board;

	std::shared_ptr<Texture> tOne;

};

