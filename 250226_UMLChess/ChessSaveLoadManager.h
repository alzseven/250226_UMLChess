#pragma once

#include <string>
#include "ChessData.h"

class ChessSaveLoadManager
{
public:
	ChessSaveLoadManager();
	~ChessSaveLoadManager();

	bool WriteFile(const std::string& fileName, const ChessSaveData gameData);
	bool ReadFile(const std::string& fileName, ChessSaveData& gameData);


	//void WriteFile(const std::string& fileName, const ChessSaveData gameData, bool isBinary);

	//void ReadFile(const std::string& fileName, ChessSaveData& gameData, bool isBinary);
};
