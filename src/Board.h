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
	void HandleMovement(TetrominoMove dir);
	bool CanMove(TetrominoMove dir);
	void KickIntoBounds(Tetromino& tetromino);
	bool IntersectsWithSettled(Tetromino tetromino);

	void CheckLines();
	void ShiftBoardDown(int row);
	void UpdateScore(int lines);

	void Draw(Shader& shader);

private:
	std::vector<std::vector<Cell>> m_Board;

	unsigned int m_VBO;
	unsigned int m_VAO;
	unsigned int m_EBO;

	std::shared_ptr<Texture> tOne;

	glm::mat4 m_Proj;

};

