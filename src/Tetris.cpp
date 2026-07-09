#include "Tetris.h"

Tetris::Tetris() 
    : m_Keys{}, m_KeysProcessed{}, m_GameTime{ (float)glfwGetTime() }
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
	m_PreviousTime = (float)glfwGetTime();

	m_Board = std::make_unique<Board>();

	m_Shader = std::make_unique<Shader>("res\\shaders\\game_elements_vertex_shader.glsl", "res\\shaders\\game_elements_frag_shader.glsl");
	m_SpriteRenderer = std::make_unique<SpriteRenderer>(*m_Shader);

	m_GameState = GAME_ACTIVE;
}

Tetris::~Tetris()
{
	glfwTerminate();
}

void Tetris::Run()
{
	while (!glfwWindowShouldClose(m_Window)) {
		glfwPollEvents();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (m_GameState == GAME_ACTIVE) {
			double currentTime = glfwGetTime();
			m_DeltaTime = currentTime - m_GameTime;

			if (m_DeltaTime >= m_GameSpeed) {
				if (m_Board->UpdateBoard()) {
					m_GameTime = currentTime;
				}
				else {
					m_GameState = GAME_OVER;
				}
			}

			ProcessInput();

			m_SpriteRenderer->SetProjection(m_Proj);
			m_Board->Draw(*m_SpriteRenderer, m_GameLayout.board);
		}

		glfwSwapBuffers(m_Window);
	}
}

void Tetris::ProcessInput() {
	float cooldown = 0.1f;
	float currentTime = (float)glfwGetTime();

	if (m_Keys[GLFW_KEY_LEFT]) {
		if (currentTime - m_PreviousTime >= cooldown) {
			std::cout << "Pressed left" << std::endl;
			m_Board->HandleMovement(LEFT);
			m_PreviousTime = currentTime;
		}
	}

	if (m_Keys[GLFW_KEY_RIGHT]) {
		if (currentTime - m_PreviousTime >= cooldown) {
			std::cout << "Pressed right" << std::endl;
			m_Board->HandleMovement(RIGHT);
			m_PreviousTime = currentTime;
		}
	}

	if (m_Keys[GLFW_KEY_UP] && !m_KeysProcessed[GLFW_KEY_UP]) {
		std::cout << "Pressed up" << std::endl;
		m_Board->HandleMovement(ROTATE);
		m_KeysProcessed[GLFW_KEY_UP] = true;
	}

	if (m_Keys[GLFW_KEY_DOWN]) {
		if (currentTime - m_PreviousTime >= cooldown) {
			std::cout << "Pressed down" << std::endl;
			m_Board->HandleMovement(DOWN);
			m_PreviousTime = currentTime;
			m_GameTime = currentTime;
		}
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

Rect Tetris::FitAspect(Rect container, float aspectW, float aspectH) {
	float containerAspect = container.w / container.h;
	float targetAspect = aspectW / aspectH;

	Rect r = container;

	if (containerAspect > targetAspect) {
		r.w = container.h * targetAspect;
		r.x = container.x + (container.w - r.w) * 0.5f;
	}
	else {
		r.h = container.w / targetAspect;
		r.y = container.y + (container.h - r.h) * 0.5f;
	}

	return r;
}

glm::mat4 RectToTransform(const Rect& r, float logicalW, float logicalH) {
	glm::mat4 m = glm::translate(glm::mat4(1.0f), glm::vec3(r.x, r.y, 0.0f));
	m = glm::scale(m, glm::vec3(r.w / logicalW, r.h / logicalH, 1.0f));
	return m;
}

GameLayout Tetris::ComputeLayout(int windowW, int windowH) {
	if (windowH <= 0) windowH = 1;

	GameLayout layout;

	float sidebarW = windowW * 0.25;
	Rect boardContainer = { 0, 0, windowW - sidebarW, (float)windowH };
	layout.board = FitAspect(boardContainer, 10.0f, 20.0f);
	layout.boardTransform = RectToTransform(layout.board, 10.0f, 20.0f);

	return layout;
}

void Tetris::FrameBufferSizeCallback(GLFWwindow* window, int width, int height) 
{
	if (width <= 0 || height <= 0) return;

	Tetris* self = static_cast<Tetris*>(glfwGetWindowUserPointer(window));

	glViewport(0, 0, width, height);

	self->m_Proj = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
	self->m_GameLayout = self->ComputeLayout(width, height);
}
