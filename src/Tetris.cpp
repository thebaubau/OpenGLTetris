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

	// Game related setup
	
	m_Board = std::make_unique<Board>();

	m_Shader = std::make_unique<Shader>("res\\shaders\\simple_vertex_shader.glsl", "res\\shaders\\simple_frag_shader.glsl");

	float triVertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};

	glGenBuffers(1, &m_VBO);
	glGenVertexArrays(1, &m_VAO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triVertices), triVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

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
				system("cls");
				if (m_Board->UpdateBoard()) {
					m_Board->PrintBoard();
					previousTime = currentTime;
				}
				else {
					m_GameState = GAME_OVER;
				}
			}
			m_Shader->Bind();
			glBindVertexArray(m_VAO);
			glDrawArrays(GL_TRIANGLES, 0, 3);

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
	glViewport(0, 0, width, height);
}
