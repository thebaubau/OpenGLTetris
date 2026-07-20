#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <map>
#include "Shader.h"

struct Character {
    unsigned int TextureID;  // ID handle of the glyph texture
    glm::ivec2   Size;       // Size of glyph
    glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
    unsigned int Advance;    // Offset to advance to next glyph
};

class TextRenderer
{
public:
	TextRenderer(std::string path);
	~TextRenderer();

    void RenderText(Shader& s, const std::string& text, float x, float y, float scale, glm::vec3 color, const glm::mat4& projection);
private:
    std::map<char, Character> m_Characters;
    unsigned int m_VAO, m_VBO;

	void SetupFont(const std::string& path);
};

