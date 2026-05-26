#pragma once

#include <glm/glm.hpp>
#include <vector>

enum TetrominoMove {
	LEFT,
	RIGHT,
	ROTATE,
	DOWN
};

class Tetromino
{
public:
	Tetromino(std::vector<std::vector<int>> tetrominoData, glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f), int rowPos = 7, int colPos = 3);
	~Tetromino();

	std::vector<std::vector<int>> m_TetrominoData;
	glm::vec3 m_Color;
	int m_RowPos;
	int m_ColPos;
	int m_Width;
	int m_Height;

	bool m_Active = false;

	void Move(TetrominoMove move);
private:

};

