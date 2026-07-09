#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer(Shader& shader)
	: m_Shader(shader)
{
	InitRenderData();
}

SpriteRenderer::~SpriteRenderer()
{
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_EBO);
	glDeleteVertexArrays(1, &m_VAO);
}

void SpriteRenderer::SetProjection(const glm::mat4& projection)
{
	m_Shader.Bind();
	m_Shader.SetMat4("projection", projection);
}

void SpriteRenderer::Draw(Texture& texture, glm::vec2 position, glm::vec2 size,
                          float rotate, glm::vec3 color)
{
	m_Shader.Bind();

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(position, 0.0f));

	// Rotate around the sprite's center rather than its corner
	model = glm::translate(model, glm::vec3(0.5f * size.x, 0.5f * size.y, 0.0f));
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::translate(model, glm::vec3(-0.5f * size.x, -0.5f * size.y, 0.0f));

	model = glm::scale(model, glm::vec3(size, 1.0f));

	m_Shader.SetMat4("model", model);
	m_Shader.SetVec3("spriteColor", color);

	texture.Bind(0);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void SpriteRenderer::InitRenderData()
{
	float vertices[] = {
		// pos        // tex
		1.0f, 1.0f,   1.0f, 1.0f,   // top right
		1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		0.0f, 0.0f,   0.0f, 0.0f,   // bottom left
		0.0f, 1.0f,   0.0f, 1.0f    // top left
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}
