#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <random>
#include <Rect.h>
#include "Tetromino.h"
#include "TetrominoDataLoader.h"
#include "SpriteRenderer.h"

struct Cell {
	int value = 0;
	glm::vec3 color{ 1.0f };
	std::shared_ptr<Texture> texture;
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
	std::unique_ptr<Tetromino> m_NextTetromino;

	int m_SpawnPostion[2] = { 0, 3 };
	
	float m_GameSpeed;
	int m_Level;

	int m_BoardScore = 0;
	int m_NumOfLines = 0;

	const std::map<int, int> m_Scores {
		{ 1, 100 },
		{ 2, 300 },
		{ 3, 500 },
		{ 4, 800 }
	};

	const std::vector<float> m_Speeds {
		0.80f, // level 1
		0.72f, // level 2
		0.63f, // level 3
		0.55f, // level 4
		0.47f, // level 5
		0.38f, // level 6
		0.30f, // level 7
		0.22f, // level 8
		0.13f, // level 9
		0.10f, // level 10
		0.083f, // level 11
		0.083f, // level 12
		0.083f, // level 13
		0.067f, // level 14
		0.067f  // level 15
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

	void UpdateGameSpeed();

	void CheckLines();
	void ShiftBoardDown(int row);
	void UpdateScore(int lines);

	void Draw(SpriteRenderer& renderer, const Rect& board, float cellSize);

	std::unique_ptr<Texture> m_BoardBg;

private:
	std::vector<std::vector<Cell>> m_Board;

	std::vector<std::shared_ptr<Texture>> m_TetrominoTextures;

};

