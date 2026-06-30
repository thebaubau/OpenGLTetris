#include "Tetris.h"

Tetris::Tetris() 
    : m_Keys{}, 
      m_KeysProcessed{}
{
	// Window setup and lib loading
	glfwSetErrorCallback(ErrorCallback);

	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	glfwWindowHint(GLFW_STENCIL_BITS, 8);
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
	//glfwWindowHint(GLFW_DECORATED, GL_FALSE);

	m_Window = glfwCreateWindow(800, 600, "Tetris", NULL, NULL);

	if (m_Window == NULL) {
		std::cerr << "Failed to load GLFW window" << std::endl;
		glfwTerminate();
	}
	
	glfwMakeContextCurrent(m_Window);
	glfwSetFramebufferSizeCallback(m_Window, FrameBufferSizeCallback);

	glfwSetWindowUserPointer(m_Window, this);
	glfwSetKeyCallback(m_Window, []
	(GLFWwindow* win, int key, int scancode, int action, int mods) {
			auto* self = static_cast<Tetris*>(glfwGetWindowUserPointer(win));
			self->KeyCallback(win, key, scancode, action, mods);
		});
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cerr << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
	}

	int fbWidth, fbHeight;
	glfwGetFramebufferSize(m_Window, &fbWidth, &fbHeight);
	FrameBufferSizeCallback(m_Window, fbWidth, fbHeight);

	// Game related setup
	
	m_Board = std::make_unique<Board>();

	//m_Shader = std::make_unique<Shader>("res\\shaders\\simple_vertex_shader.glsl", "res\\shaders\\simple_frag_shader.glsl");

	m_Shader = std::make_unique<Shader>("res\\shaders\\game_elements_vertex_shader.glsl", "res\\shaders\\game_elements_frag_shader.glsl");

	m_GameState = GAME_ACTIVE;
}

Tetris::~Tetris()
{
	glfwTerminate();
}

void Tetris::Run()
{
	double previousTime = glfwGetTime();

	while (!glfwWindowShouldClose(m_Window)) {
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (m_GameState == GAME_ACTIVE) {
			double currentTime = glfwGetTime();
			double deltaTime = currentTime - previousTime;

			ProcessInput();

			if (deltaTime >= m_GameSpeed) {
				if (m_Board->UpdateBoard()) {
					previousTime = currentTime;
				}
				else {
					m_GameState = GAME_OVER;
				}
			}

			m_Board->Draw(*m_Shader);
		}

		glfwSwapBuffers(m_Window);
	}
}

void Tetris::ProcessInput() {
	if (m_Keys[GLFW_KEY_LEFT] && !m_KeysProcessed[GLFW_KEY_LEFT]) {
		std::cout << "Pressed left" << std::endl;
		m_Board->HandleMovement(LEFT);
		m_KeysProcessed[GLFW_KEY_LEFT] = true;
	}
	if (m_Keys[GLFW_KEY_RIGHT] && !m_KeysProcessed[GLFW_KEY_RIGHT]) {
		std::cout << "Pressed right" << std::endl;
		m_Board->HandleMovement(RIGHT);
		m_KeysProcessed[GLFW_KEY_RIGHT] = true;
	}
	if (m_Keys[GLFW_KEY_UP] && !m_KeysProcessed[GLFW_KEY_UP]) {
		std::cout << "Pressed up" << std::endl;
		m_Board->HandleMovement(ROTATE);
		m_KeysProcessed[GLFW_KEY_UP] = true;
	}
	if (m_Keys[GLFW_KEY_DOWN] && !m_KeysProcessed[GLFW_KEY_DOWN]) {
		std::cout << "Pressed down" << std::endl;
		m_Board->HandleMovement(DOWN);
		m_KeysProcessed[GLFW_KEY_DOWN] = true;
	}
}

void Tetris::ErrorCallback(int error, const char* description)
{
	fprintf(stderr, "Error: %s\n", description);
}

void Tetris::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			m_Keys[key] = true;
		else if (action == GLFW_RELEASE)
		{
			m_Keys[key] = false;
			m_KeysProcessed[key] = false;
		}
	}
}

void Tetris::FrameBufferSizeCallback(GLFWwindow* window, int width, int height) {
	float targetAspect = 10.0f / 20.0f;
	float windowAspect = (float)width / (float)height;

	int boardW = width;
	int boardH = height;
	int offsetX, offsetY;

	if (windowAspect > targetAspect) {
		boardW = boardH * targetAspect;

		offsetX = ((float)width - boardW) / 2;
		offsetY = ((float)height - boardH) / 2;
	}
	else {
		boardH = boardW / targetAspect;

		offsetX = ((float)width - boardW) / 2;
		offsetY = ((float)height - boardH) / 2;
	}

	glViewport(offsetX, offsetY, boardW, boardH);
}
