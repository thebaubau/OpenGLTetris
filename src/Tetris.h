#pragma once
#include <stdlib.h>
#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "Shader.h"
#include "Board.h"
#include "Texture.h"
#include "NextTetromino.h"

enum GameState {
	GAME_ACTIVE,
	GAME_OVER,
	GAME_WIN
};

struct GameLayout {
	Rect board;
	Rect gameBg;
	Rect nextTetromino;

	// Pixels per block. Single source of truth: the board and the
	// preview panel must render blocks at the same size.
	float cellSize = 0.0f;
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
	std::unique_ptr<NextTetromino> m_Next;
private:
	GLFWwindow* m_Window = nullptr;

	double m_DeltaTime;
	float m_GameTime;

	bool m_Keys[1024];
	bool m_KeysProcessed[1024];
	float m_NextMoveTime[1024]{};
	int m_ActiveHorizontalKey = 0;

	std::unique_ptr<Shader> m_Shader;
	std::unique_ptr<Texture> m_GameBackground;
	std::unique_ptr<SpriteRenderer> m_SpriteRenderer;

	GameLayout m_GameLayout;
	int m_WindowW, m_WindowH;
	glm::mat4 m_Proj;

	bool HandleKeyRepeat(int key, TetrominoMove dir, float firstDelay, float repeatDelay, float now);
	void ProcessInput();
	Rect FitAspect(Rect container, float aspectW, float aspectH);
	Rect FitCover(Rect container, float aspectW, float aspectH);
	GameLayout ComputeLayout(int windowW, int windowH, int imageW, int imageH);

	void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void ErrorCallback(int error, const char* description);
	static void FrameBufferSizeCallback(GLFWwindow* window, int width, int height);
};

