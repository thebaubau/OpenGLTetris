#pragma once
#include <stdlib.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Board.h"
#include "Texture.h"

enum GameState {
	GAME_ACTIVE,
	GAME_OVER,
	GAME_WIN
};

class Tetris
{
public:
	Tetris();
	~Tetris();

	void Run();

	int m_GameSpeed = 1;
	GameState m_GameState;

	std::unique_ptr<Board> m_Board;
private:
	GLFWwindow* m_Window = nullptr;

	double m_DeltaTime;
	float m_GameTime;
	float m_PreviousTime;

	bool m_Keys[1024];
	bool m_KeysProcessed[1024];
	std::unique_ptr<Shader> m_Shader;

	void ProcessInput();

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void ErrorCallback(int error, const char* description);
	static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
};

