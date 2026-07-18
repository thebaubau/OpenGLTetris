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

	m_WindowH = 0;
	m_WindowW = 0;
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

	m_GameBackground = std::make_unique<Texture>("res\\textures\\game_bg.png");

	glfwGetFramebufferSize(m_Window, &m_WindowW, &m_WindowH);
	FrameBufferSizeCallback(m_Window, m_WindowW, m_WindowH);

	// Game related setup
	m_Board = std::make_unique<Board>();
	m_Next = std::make_unique<NextTetromino>();

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

			m_SpriteRenderer->Draw(
				*m_GameBackground, 
				glm::vec2(m_GameLayout.gameBg.x, m_GameLayout.gameBg.y), 
				glm::vec2(m_GameLayout.gameBg.w, m_GameLayout.gameBg.h));

			m_Board->Draw(*m_SpriteRenderer, m_GameLayout.board, m_GameLayout.cellSize);
			
			if (m_Board->m_NextTetromino != nullptr) {
				m_Next->Draw(*m_SpriteRenderer, m_GameLayout.nextTetromino, *m_Board->m_NextTetromino, m_GameLayout.cellSize);
			}
		}

		glfwSwapBuffers(m_Window);
	}
}

bool Tetris::HandleKeyRepeat(int key, TetrominoMove dir, float firstDelay, float repeatDelay, float now) {
	if (!m_KeysProcessed[key]) {
		m_KeysProcessed[key] = true;
		m_NextMoveTime[key] = now + firstDelay;
		return m_Board->HandleMovement(dir);
	}

	else if (now >= m_NextMoveTime[key]) {
		m_NextMoveTime[key] = now + repeatDelay;
		return m_Board->HandleMovement(dir);
	}

	return false;
}

void Tetris::ProcessInput() {
	float cooldown = 0.07f;
	float cooldownDAS = 0.17f;
	float currentTime = (float)glfwGetTime();

	if (m_ActiveHorizontalKey != 0) {
		TetrominoMove dir = (m_ActiveHorizontalKey == GLFW_KEY_LEFT) ? LEFT : RIGHT;
		HandleKeyRepeat(m_ActiveHorizontalKey, dir, cooldownDAS, cooldown, currentTime);
	}

	if (m_Keys[GLFW_KEY_UP] && !m_KeysProcessed[GLFW_KEY_UP]) {
		m_Board->HandleMovement(ROTATE);
		m_KeysProcessed[GLFW_KEY_UP] = true;
	}

	if (m_Keys[GLFW_KEY_DOWN]) {
		if (HandleKeyRepeat(GLFW_KEY_DOWN, DOWN, cooldown, cooldown, currentTime))
			m_GameTime = currentTime;
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

		if (key == GLFW_KEY_LEFT || key == GLFW_KEY_RIGHT)
		{
			int other = (key == GLFW_KEY_LEFT) ? GLFW_KEY_RIGHT : GLFW_KEY_LEFT;

			if (action == GLFW_PRESS)
			{
				m_ActiveHorizontalKey = key;
				m_KeysProcessed[key] = false;
			}
			else if (action == GLFW_RELEASE && key == m_ActiveHorizontalKey)
			{
				if (m_Keys[other])
				{
					m_ActiveHorizontalKey = other;
					m_KeysProcessed[other] = false;
				}
				else
					m_ActiveHorizontalKey = 0;
			}
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

Rect Tetris::FitCover(Rect container, float aspectW, float aspectH) {
    float containerAspect = container.w / container.h;
    float targetAspect = aspectW / aspectH;

    Rect r;

    if (containerAspect > targetAspect) {
        r.w = container.w;
        r.h = container.w / targetAspect;
    } else {
        r.h = container.h;
        r.w = container.h * targetAspect;
    }

    r.x = container.x + (container.w - r.w) / 2.0f;
    r.y = container.y + (container.h - r.h) / 2.0f;

    return r;
}

GameLayout Tetris::ComputeLayout(int windowW, int windowH, int imageW, int imageH) {
	if (windowH <= 0) windowH = 1;

	GameLayout layout;

	Rect gameBackground = { 0, 0, (float)windowW, (float)windowH };
	layout.gameBg = FitCover(gameBackground, (float)imageW, (float)imageH);

	const float padY = 30.0f;

	Rect boardContainer = { 0.0f, padY, (float)windowW, windowH - 2.0f * padY };
	layout.board = FitAspect(boardContainer, 10.0f, 20.0f);

	// One block in pixels. Everything else is a multiple of this.
	layout.cellSize = layout.board.w / 10.0f;

	const float panelSize = 4.0f * layout.cellSize;
	const float nextX = layout.board.x + layout.board.w + layout.cellSize;

	layout.nextTetromino = {
		nextX,
		layout.board.y,
		panelSize,
		panelSize
	};

	return layout;
}

void Tetris::FrameBufferSizeCallback(GLFWwindow* window, int width, int height) 
{
	if (width <= 0 || height <= 0) return;

	Tetris* self = static_cast<Tetris*>(glfwGetWindowUserPointer(window));

	glViewport(0, 0, width, height);
	self->m_WindowW = width;
	self->m_WindowH = height;

	self->m_Proj = glm::ortho(0.0f, (float)width, (float)height, 0.0f, -1.0f, 1.0f);
	self->m_GameLayout = self->ComputeLayout(width, height, self->m_GameBackground->GetWidth(), self->m_GameBackground->GetHeight());
}
