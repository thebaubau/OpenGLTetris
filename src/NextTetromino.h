#pragma once
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <Rect.h>
#include "SpriteRenderer.h"
#include "Tetromino.h"

class NextTetromino
{
public:
	NextTetromino();
	~NextTetromino();

	void Draw(SpriteRenderer& renderer, const Rect& rect, const Tetromino& next, float cellSize);
	std::unique_ptr<Texture> m_PanelBg;

private:
};

