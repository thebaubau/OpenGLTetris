#include "NextTetromino.h"

NextTetromino::NextTetromino()
{
	m_BoardBg = std::make_unique<Texture>("res\\textures\\panel_bg.jpg");
}

NextTetromino::~NextTetromino()
{}

void NextTetromino::Draw(SpriteRenderer & renderer, const Rect& rect, const Tetromino& next, float cellSize)
{
	// Board background, inset by a small margin (in cells) around the grid.
	float margin = 0.2f;
	glm::vec2 bgPos = { rect.x - margin * cellSize, rect.y - margin * cellSize };
	glm::vec2 bgSize = { rect.w + 2.0f * margin * cellSize, rect.h + 2.0f * margin * cellSize };

	// Panel background.
	renderer.Draw(*m_BoardBg, bgPos, bgSize);

	const int rows = (int)next.m_TetrominoData.size();
	const int cols = (int)next.m_TetrominoData[0].size();

	// Blocks are drawn at the board's cell size rather than scaled to fit the
	// panel, so the preview matches the piece that will actually spawn.
	for (int i{ 0 }; i < rows; ++i) {
		for (int j{ 0 }; j < cols; ++j) {
			if (next.m_TetrominoData[i][j] != 0) {
				glm::vec2 pos = { rect.x + j * cellSize, rect.y + i * cellSize };
				renderer.Draw(*next.m_Texture, pos, { cellSize, cellSize });
			}
		}
	}
}
