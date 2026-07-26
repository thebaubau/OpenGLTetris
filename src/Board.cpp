#include "Board.h"

Board::Board()
	: m_Board(20, std::vector<Cell>(10))
{
	std::cout << "Constructing board" << std::endl;

	m_TetrominoTextures.push_back(std::make_shared<Texture>("res\\textures\\tetromino_blue.jpg"));
	m_TetrominoTextures.push_back(std::make_shared<Texture>("res\\textures\\tetromino_less_orange.jpg"));
	m_TetrominoTextures.push_back(std::make_shared<Texture>("res\\textures\\tetromino_green.jpg"));
	m_TetrominoTextures.push_back(std::make_shared<Texture>("res\\textures\\tetromino_pink.jpg"));
	m_TetrominoTextures.push_back(std::make_shared<Texture>("res\\textures\\tetromino_purple.jpg"));
	m_TetrominoTextures.push_back(std::make_shared<Texture>("res\\textures\\tetromino_red.jpg"));
	m_TetrominoTextures.push_back(std::make_shared<Texture>("res\\textures\\tetromino_teal.jpg"));
	
	// Loading all tetromino data
	m_Tetrominos.push_back(Tetromino(TetrominoDataLoader::LoadTetrominoData("tetromino_l.txt"), m_TetrominoTextures[0]));
	m_Tetrominos.push_back(Tetromino(TetrominoDataLoader::LoadTetrominoData("tetromino_ll.txt"), m_TetrominoTextures[1]));
	m_Tetrominos.push_back(Tetromino(TetrominoDataLoader::LoadTetrominoData("tetromino_i.txt"), m_TetrominoTextures[2]));
	m_Tetrominos.push_back(Tetromino(TetrominoDataLoader::LoadTetrominoData("tetromino_z.txt"), m_TetrominoTextures[3]));
	m_Tetrominos.push_back(Tetromino(TetrominoDataLoader::LoadTetrominoData("tetromino_s.txt"), m_TetrominoTextures[4]));
	m_Tetrominos.push_back(Tetromino(TetrominoDataLoader::LoadTetrominoData("tetromino_sq.txt"), m_TetrominoTextures[5]));
	m_Tetrominos.push_back(Tetromino(TetrominoDataLoader::LoadTetrominoData("tetromino_t.txt"), m_TetrominoTextures[6]));

	m_BoardBg = std::make_unique<Texture>("res\\textures\\board_bg.jpg");
	m_Level = 0;
	UpdateGameSpeed();
}

Board::~Board()
{}

bool Board::UpdateBoard()
{
	if (m_ActiveTetromino == nullptr || m_ActiveTetromino->m_Active == false) {
		if (!SpawnTetromino()) return false;
	}
	else {
		HandleMovement(DOWN);
	}
	return true;
}

bool Board::SpawnTetromino()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(0, m_Tetrominos.size() - 1);

	if (m_NextTetromino != nullptr) {
		m_ActiveTetromino = std::move(m_NextTetromino);
	}
	else {
		m_ActiveTetromino = std::make_unique<Tetromino>(m_Tetrominos[distr(gen)]);
	}

	m_NextTetromino = std::make_unique<Tetromino>(m_Tetrominos[distr(gen)]);

	if (IntersectsWithSettled(*m_ActiveTetromino)) {
		return false;
	}
	
	m_ActiveTetromino->m_Active = true;
	return true;
}

void Board::DrawBoard() {
	ClearBoard();

	if (m_ActiveTetromino == nullptr || m_ActiveTetromino->m_Active == false) return;

	int boardRowSize = m_Board.size() - 1;
	int boardColSize = m_Board[0].size() - 1;

	for (int i = 0; i < m_ActiveTetromino->m_Height; i++) {
		for (int j = 0; j < m_ActiveTetromino->m_Width; j++) {
			if (m_ActiveTetromino->m_ColPos + j >= 0 && m_ActiveTetromino->m_ColPos + j <= boardColSize && m_ActiveTetromino->m_RowPos + i <= boardRowSize && m_ActiveTetromino->m_TetrominoData[i][j] == 1) {
				m_Board[m_ActiveTetromino->m_RowPos + i][m_ActiveTetromino->m_ColPos + j].value = 1;
			}
		}
	}
}

void Board::LockTetromino()
{
	for (int i = 0; i < m_ActiveTetromino->m_Height; i++) {
		for (int j = 0; j < m_ActiveTetromino->m_Width; j++) {
			if (m_ActiveTetromino->m_TetrominoData[i][j] == 1) {
				Cell& cell = m_Board[m_ActiveTetromino->m_RowPos + i][m_ActiveTetromino->m_ColPos + j];
				cell.value = 2;
				cell.texture = m_ActiveTetromino->m_Texture;
			}
		}
	}
	std::cout << "Tetromino Locked" << std::endl;
}

void Board::ClearBoard()
{
	for (int i = 0; i < m_Board.size(); i++) {
		for (int j = 0; j < m_Board[i].size(); j++) {
			if (m_Board[i][j].value == 2)
				continue;
			m_Board[i][j].value = 0;
		}
	}
}

bool Board::HandleMovement(TetrominoMove dir)
{
	if (m_ActiveTetromino == nullptr || m_ActiveTetromino->m_Active == false) return false;

	if (dir == LEFT && CanMove(LEFT)) {
		std::cout << "Moving left" << std::endl;
		m_ActiveTetromino->Move(LEFT);
		return true;
	}

	if (dir == RIGHT && CanMove(RIGHT)) {
		std::cout << "Moving right" << std::endl;
		m_ActiveTetromino->Move(RIGHT);
		return true;
	}

	if (dir == DOWN) {
		if (CanMove(DOWN)) {
			std::cout << "Moving down" << std::endl;
			m_ActiveTetromino->Move(DOWN);
			return true;
		}
		else {
			LockTetromino();
			m_ActiveTetromino->m_Active = false;
			CheckLines();
			return false;
		}
	}

	if (dir == ROTATE) {
		int tempCol = m_ActiveTetromino->m_ColPos;
		int tempRow = m_ActiveTetromino->m_RowPos;
		std::vector<std::vector<int>> tempData = m_ActiveTetromino->m_TetrominoData;

		Tetromino temp(tempData, nullptr);
		temp.m_ColPos = tempCol;
		temp.m_RowPos = tempRow;

		temp.Move(ROTATE);
		KickIntoBounds(temp);

		if (!IntersectsWithSettled(temp)) {
			std::cout << "Rotating" << std::endl;
			m_ActiveTetromino->m_ColPos = temp.m_ColPos;
			m_ActiveTetromino->m_RowPos = temp.m_RowPos;
			m_ActiveTetromino->Move(ROTATE);
			return true;
		}
	}

	if (dir == DROP) {
		std::cout << "Hard drop" << std::endl;
		while (CanMove(DOWN)) {
			m_ActiveTetromino->Move(DOWN);
		}

		LockTetromino();
		m_ActiveTetromino->m_Active = false;
		CheckLines();

		return true;
	}

	return false;
}

bool Board::CanMove(TetrominoMove dir)
{
	switch (dir)
	{
	case TetrominoMove::LEFT:
		for (int i = 0; i < m_ActiveTetromino->m_Height; ++i) {
			for (int j = 0; j < m_ActiveTetromino->m_Width; ++j) {
				int squareValue = m_ActiveTetromino->m_TetrominoData[i][j];

				if (squareValue != 1)
					continue;

				int leftEdge = m_ActiveTetromino->m_ColPos + j - 1;
				if (leftEdge < 0 || m_Board[m_ActiveTetromino->m_RowPos + i][leftEdge].value == 2)
				{
					return false;
				}
			}
		}
		break;
	
	case TetrominoMove::RIGHT:
		for (int i = 0; i < m_ActiveTetromino->m_Height; ++i) {
			for (int j = m_ActiveTetromino->m_Width - 1; j >= 0; --j) {
				int squareValue = m_ActiveTetromino->m_TetrominoData[i][j];
				if (squareValue != 1)
					continue;

				int rightEdge = m_ActiveTetromino->m_ColPos + j + 1;
				if (rightEdge >= m_Board[0].size() || m_Board[m_ActiveTetromino->m_RowPos + i][rightEdge].value == 2)
				{
					return false;
				}
			}
		}
		break;

	case TetrominoMove::DOWN:
		for (int i = m_ActiveTetromino->m_Height - 1; i >= 0 ; --i) {
			for (int j = m_ActiveTetromino->m_Width - 1; j >= 0; --j) {
				int squareValue = m_ActiveTetromino->m_TetrominoData[i][j];
				if (squareValue != 1)
					continue;

				int bottomEdge = m_ActiveTetromino->m_RowPos + i + 1;
				if (bottomEdge >= m_Board.size() || m_Board[bottomEdge][m_ActiveTetromino->m_ColPos + j].value == 2)
				{
					return false;
				}
			}
		}

		break;
	default:
		std::cerr << "Must specify correct direction." << std::endl;
		break;
	}

	return true;
}

void Board::KickIntoBounds(Tetromino& piece) {
	const int pieceWidth = piece.m_Width;
	const int boardWidth = m_Board[0].size();

	while (piece.m_ColPos < 0) piece.Move(RIGHT);
	while (piece.m_ColPos + pieceWidth > boardWidth) piece.Move(LEFT);
}

bool Board::IntersectsWithSettled(Tetromino tetromino)
{
	for (int i{ 0 }; i < tetromino.m_Width; ++i) {
		for (int j{ 0 }; j < tetromino.m_Width; ++j) {
			if (tetromino.m_TetrominoData[i][j] == 0)
				continue;
			if (tetromino.m_RowPos + i > m_Board.size() - 1)
				return true;
			if (m_Board[tetromino.m_RowPos + i][tetromino.m_ColPos + j].value == 2)
				return true;
		}
	}
	return false;
}

void Board::UpdateGameSpeed()
{
	if (m_Level >= (int)m_Speeds.size() - 1)
		m_GameSpeed = m_Speeds.back();
	else
		m_GameSpeed = m_Speeds[m_Level];
}

void Board::CheckLines()
{
	int settledValue = 2;
	int numOfLinesScored = 0;

	for (int i{ (int)m_Board.size() - 1 }; i > 0; i--) {
		if (std::all_of(m_Board[i].begin(), m_Board[i].end(), [settledValue](Cell n) { return n.value == settledValue; })) {
			numOfLinesScored++;
			ShiftBoardDown(i);
			i++;
		}
	}

	if (numOfLinesScored == 0) return;

	UpdateScore(numOfLinesScored);
}

void Board::ShiftBoardDown(int row)
{
	for (int r = 0 + row; r > 0; --r) {
		m_Board[r] = m_Board[r - 1];
	}

	std::fill(m_Board[0].begin(), m_Board[0].end(), Cell{});
}

void Board::UpdateScore(int lines)
{
	m_NumOfLines += lines;
	m_BoardScore += m_Scores.at(lines);

	m_Level = m_NumOfLines / 10;
	UpdateGameSpeed();
}

void Board::Draw(SpriteRenderer& renderer, const Rect& board, float cellSize)
{
	DrawBoard();

	// Board background, inset by a small margin (in cells) around the grid.
	float margin = 0.2f;

	glm::vec2 bgPos = { board.x - margin * cellSize, board.y - margin * cellSize };
	glm::vec2 bgSize = { board.w + 2.0f * margin * cellSize, board.h + 2.0f * margin * cellSize };
	renderer.Draw(*m_BoardBg, bgPos, bgSize);

	// Board grid / tetrominos
	for (int i{ 0 }; i < 20; ++i) {
		for (int j{ 0 }; j < 10; ++j) {
			int cell = m_Board[i][j].value;
			
			if (cell == 1 || cell == 2) {
				glm::vec2 pos = { board.x + j * cellSize, board.y + i * cellSize };
				if (cell == 1) {
					renderer.Draw(*m_ActiveTetromino->m_Texture, pos, { cellSize, cellSize });
				}
				else {
					renderer.Draw(*m_Board[i][j].texture, pos, {cellSize, cellSize});
				}
			}
		
		}
	}
}
