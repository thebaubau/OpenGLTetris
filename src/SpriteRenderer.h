#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Texture.h"

class SpriteRenderer
{
public:
	SpriteRenderer(Shader& shader);
	~SpriteRenderer();

	// Set once per frame (projection is pixel-space and only changes on resize)
	void SetProjection(const glm::mat4& projection);

	void Draw(Texture& texture, glm::vec2 position, glm::vec2 size,
	          float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

private:
	// Borrowed, not owned — Tetris owns the shader.
	Shader& m_Shader;

	unsigned int m_VAO = 0;
	unsigned int m_VBO = 0;
	unsigned int m_EBO = 0;

	void InitRenderData();
};
