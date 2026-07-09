#include "Texture.h"

Texture::Texture(std::string path, std::string type)
	: m_FilePath(path), m_Type(type), m_Width(0), m_Height(0)
{
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_NrChannels, 4);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer);
	glGenerateMipmap(GL_TEXTURE_2D);

	Unbind();

	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);

	m_LocalBuffer = nullptr;
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(unsigned int slot) {
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}

int Texture::GetWidth() const
{
	return m_Width;
}

int Texture::GetHeight() const
{
	return m_Height;
}
