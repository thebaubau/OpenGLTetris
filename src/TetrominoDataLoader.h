#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

class TetrominoDataLoader
{
public:
	static std::vector<std::vector<int>> LoadTetrominoData(std::string filePath);
};

