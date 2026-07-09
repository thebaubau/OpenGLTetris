#include "Board.h"

Board::Board()
	: m_Board(20, std::vector<Cell>(10))
{
	std::cout << "Constructing board" << std::endl;

	tOne = std::make_shared<Texture>("res\\textures\\block_sprite_1.png");

	// Loading all tetromino data
	m_Tetrominos.push_back(Tetromino(TetrominoDataLoader::LoadTetrominoData("tetromino_l.txt"), tOne));
	m_Tetrominos.push_back(Tetromino(TetrominoDataLoader::LoadTetrominoData("tetromino_ll.txt"), tOne));
	m_Tetrominos.push_back(Tetromino(TetrominoDataLoader::LoadTetrominoData("tetromino_i.txt"), tOne));
	m_Tetrominos.push_back(Tetromino(TetrominoDataLoader::LoadTetrominoData("tetromino_z.txt"), tOne));
	m_Tetrominos.push_back(Tetromino(TetrominoDataLoader::LoadTetrominoData("tetromino_s.txt"), tOne));
	m_Tetrominos.push_back(Tetromino(TetrominoDataLoader::LoadTetrominoData("tetromino_sq.txt"), tOne));
	m_Tetrominos.push_back(Tetromino(TetrominoDataLoader::LoadTetrominoData("tetromino_t.txt"), tOne));

	m_BoardBg = std::make_unique<Texture>("res\\textures\\board_bg.jpg");
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
	Tetromino randomValue = m_Tetrominos[distr(gen)];

	m_ActiveTetromino = std::make_unique<Tetromino>(randomValue);
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
				m_Board[m_ActiveTetromino->m_RowPos + i][m_ActiveTetromino->m_ColPos + j].value = 2;
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

void Board::HandleMovement(TetrominoMove dir)
{
	if (m_ActiveTetromino == nullptr || m_ActiveTetromino->m_Active == false) return;

	if (dir == LEFT && CanMove(LEFT)) {
		std::cout << "Moving left" << std::endl;
		m_ActiveTetromino->Move(LEFT);
	}

	if (dir == RIGHT && CanMove(RIGHT)) {
		std::cout << "Moving right" << std::endl;
		m_ActiveTetromino->Move(RIGHT);
	}

	if (dir == DOWN && CanMove(DOWN)) {
		std::cout << "Moving down" << std::endl;
		m_ActiveTetromino->Move(DOWN);
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
		}
	}
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
					LockTetromino();
					m_ActiveTetromino->m_Active = false;
					CheckLines();

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
	m_BoardScore += m_Scores.at(lines);
}

void Board::Draw(SpriteRenderer& renderer, const Rect& board)
{
	DrawBoard();

	// One grid cell in pixels.
	float cellW = board.w / 10.0f;
	float cellH = board.h / 20.0f;

	// Board background, inset by a small margin (in cells) around the grid.
	float margin = 0.1f;
	glm::vec2 bgPos = { board.x - margin * cellW, board.y - margin * cellH };
	glm::vec2 bgSize = { board.w + 2.0f * margin * cellW, board.h + 2.0f * margin * cellH };
	renderer.Draw(*m_BoardBg, bgPos, bgSize);

	// Board grid / tetrominos
	for (int i{ 0 }; i < 20; ++i) {
		for (int j{ 0 }; j < 10; ++j) {
			if (m_Board[i][j].value == 1 || m_Board[i][j].value == 2) {
				glm::vec2 pos = { board.x + j * cellW, board.y + i * cellH };
				renderer.Draw(*tOne, pos, { cellW, cellH });
			}
		}
	}
}
