#include "TetrominoDataLoader.h"

std::vector<std::vector<int>> TetrominoDataLoader::LoadTetrominoData(std::string fileName)
{
    std::ifstream inputFile("res\\tetromino_data\\" + fileName);
    std::vector<std::vector<int>> data;

    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open the file!" << std::endl;
        return {};
    }

    unsigned int tileCode;
    std::string line;

    while (std::getline(inputFile, line)) {
        std::istringstream sstream(line);
        std::vector<int> row;

        while (sstream >> tileCode)
            row.push_back(tileCode);
        if (!row.empty())
            data.push_back(row);
    }

    return data;
}
