#pragma once
#include <string>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <stb_image.h>

class Texture
{
public:
	Texture(std::string path, std::string type = "Diffuse");
	~Texture();

	void Bind(unsigned int slot);
	void Unbind();

	std::string m_FilePath;
	std::string m_Type;
	unsigned char* m_LocalBuffer;


private:
	unsigned int m_RendererID;
	int m_Width;
	int m_Height;
	int m_NrChannels;
};

