#include "Tetromino.h"

Tetromino::Tetromino(std::vector<std::vector<int>> tetrominoData, std::shared_ptr<Texture> texture, glm::vec3 color, int rowPos, int colPos)
	:
	m_TetrominoData(tetrominoData), m_Color(color),
	m_RowPos(rowPos), m_ColPos(colPos),
	m_Width(tetrominoData.size()), m_Height(tetrominoData[0].size()),
	m_Texture(texture)
{


}

Tetromino::~Tetromino()
{}

void Tetromino::Move(TetrominoMove move)
{
	if (move == TetrominoMove::LEFT) {
		m_ColPos -= 1;
	}

	else if (move == TetrominoMove::RIGHT) {
		m_ColPos += 1;
	}

	else if (move == TetrominoMove::DOWN) {
		m_RowPos += 1;
	}

	// For now we only do clockwise
	else if (move == TetrominoMove::ROTATE) {
		std::vector<std::vector<int>> tempData = m_TetrominoData;
		
		for (int i = 0; i < m_TetrominoData.size(); ++i) {
			for (int j = 0; j < m_TetrominoData[0].size(); ++j) {
				m_TetrominoData[i][j] = tempData[j][tempData.size() - 1 - i];
			}
		}
	}
}
