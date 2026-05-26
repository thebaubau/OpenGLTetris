#include "Board.h"

Board::Board()
	: m_Board(20, std::vector<int>(10, 0))
{
	std::cout << "Constructing board" << std::endl;

	// Loading all tetromino data
	m_Tetrominos.push_back(Tetromino(TetrominoDataLoader::LoadTetrominoData("tetromino_l.txt")));
	m_Tetrominos.push_back(Tetromino(TetrominoDataLoader::LoadTetrominoData("tetromino_ll.txt")));
	m_Tetrominos.push_back(Tetromino(TetrominoDataLoader::LoadTetrominoData("tetromino_i.txt")));
	m_Tetrominos.push_back(Tetromino(TetrominoDataLoader::LoadTetrominoData("tetromino_z.txt")));
	m_Tetrominos.push_back(Tetromino(TetrominoDataLoader::LoadTetrominoData("tetromino_s.txt")));
	m_Tetrominos.push_back(Tetromino(TetrominoDataLoader::LoadTetrominoData("tetromino_sq.txt")));
	m_Tetrominos.push_back(Tetromino(TetrominoDataLoader::LoadTetrominoData("tetromino_t.txt")));
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
	DrawBoard();
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

	if (m_ActiveTetromino->m_Active == false) return;

	for (int i = 0; i < m_ActiveTetromino->m_Height; i++) {
		for (int j = 0; j < m_ActiveTetromino->m_Width; j++) {
			int boardRowSize = m_Board.size() - 1;
			int boardColSize = m_Board[0].size() - 1;

			if (m_ActiveTetromino->m_ColPos + j >= 0 && m_ActiveTetromino->m_ColPos + j <= boardColSize && m_ActiveTetromino->m_RowPos + i <= boardRowSize && m_ActiveTetromino->m_TetrominoData[i][j] == 1) {
				m_Board[m_ActiveTetromino->m_RowPos + i][m_ActiveTetromino->m_ColPos + j] = 1;
			}
		}
	}
}

void Board::LockTetromino()
{
	for (int i = 0; i < m_ActiveTetromino->m_Height; i++) {
		for (int j = 0; j < m_ActiveTetromino->m_Width; j++) {
			if (m_ActiveTetromino->m_TetrominoData[i][j] == 1) {
				m_Board[m_ActiveTetromino->m_RowPos + i][m_ActiveTetromino->m_ColPos + j] = 2;
			}
		}
	}
}

void Board::ClearBoard()
{
	for (int i = 0; i < m_Board.size(); i++) {
		for (int j = 0; j < m_Board[i].size(); j++) {
			if (m_Board[i][j] == 2)
				continue;
			m_Board[i][j] = 0;
		}
	}
}

void Board::HandleMovement(TetrominoMove dir)
{
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

		Tetromino temp(tempData);
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
				if (leftEdge < 0 || m_Board[m_ActiveTetromino->m_RowPos + i][leftEdge] == 2)
				{
					return false;
				}
			}
		}
		break;
	
	// Right wall
	case TetrominoMove::RIGHT:
		for (int i = 0; i < m_ActiveTetromino->m_Height; ++i) {
			for (int j = m_ActiveTetromino->m_Width - 1; j >= 0; --j) {
				int squareValue = m_ActiveTetromino->m_TetrominoData[i][j];
				if (squareValue != 1)
					continue;

				int rightEdge = m_ActiveTetromino->m_ColPos + j + 1;
				if (rightEdge >= m_Board[0].size() || m_Board[m_ActiveTetromino->m_RowPos + i][rightEdge] == 2)
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
				if (bottomEdge >= m_Board.size() || m_Board[bottomEdge][m_ActiveTetromino->m_ColPos + j] == 2)
				{
					LockTetromino();
					m_ActiveTetromino->m_Active = false;

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
			if (tetromino.m_TetrominoData[i][j] == 1)
				continue;
			if (tetromino.m_RowPos + i > m_Board.size() - 1)
				return true;
			if (m_Board[tetromino.m_RowPos + i][tetromino.m_ColPos + j] == 2)
				return true;
		}
	}
	return false;
}

void Board::PrintBoard()
{
	std::cout << "=~=~=~=~=~=~=~=~=~=" << std::endl;
	for (const auto& row : m_Board) {
		for (int value : row) {
			std::cout << value << " ";
		}
		std::cout << std::endl;
	}
	std::cout << "=~=~=~=~=~=~=~=~=~=" << std::endl;
}
