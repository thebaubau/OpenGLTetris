#include "NextTetromino.h"

NextTetromino::NextTetromino()
{
	m_PanelBg = std::make_unique<Texture>("res\\textures\\panel_bg.jpg");
}

NextTetromino::~NextTetromino()
{}

void NextTetromino::Draw(SpriteRenderer & renderer, const Rect& rect, const Tetromino& next, float cellSize)
{
	// Panel background.
	float margin = 0.2f;

	glm::vec2 bgPos = { rect.x - margin * cellSize, rect.y - margin * cellSize };
	glm::vec2 bgSize = { rect.w + 2.0f * margin * cellSize, rect.h + 2.0f * margin * cellSize };

	renderer.Draw(*m_PanelBg, bgPos, bgSize);

	const int rows = (int)next.m_TetrominoData.size();

	if (rows == 0) return;

	const int cols = (int)next.m_TetrominoData[0].size();

	// Next Tetromino
	int minRow = rows;
	int maxRow = -1;
	int minCol = cols;
	int maxCol = -1;

	for (int i{ 0 }; i < rows; ++i) {
		for (int j{ 0 }; j < cols; ++j) {
			if (next.m_TetrominoData[i][j] != 0) {
				minRow = std::min(minRow, i);
				maxRow = std::max(maxRow, i);
				minCol = std::min(minCol, j);
				maxCol = std::max(maxCol, j);
			}
		}
	}

	if (maxRow < minRow) return;

	float bboxW = (maxCol - minCol + 1) * cellSize;
	float bboxH = (maxRow - minRow + 1) * cellSize;

	float originX = rect.x + (rect.w - bboxW) * 0.5f;
	float originY = rect.y + (rect.h - bboxH) * 0.5f;

	for (int i{ 0 }; i < rows; ++i) {
		for (int j{ 0 }; j < cols; ++j) {
			if (next.m_TetrominoData[i][j] != 0) {
				glm::vec2 pos = {
					originX + (j - minCol) * cellSize,
					originY + (i - minRow) * cellSize };
				renderer.Draw(*next.m_Texture, pos, { cellSize, cellSize });
			}
		}
	}
}
