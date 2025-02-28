#pragma once
#include <queue>

#include <fstream>
#include <windows.h>
#include <iostream>
#include "ChessData.h"
#include "ChessSaveLoadManager.h"
#include "Board.h"
#include "Unit.h"



class GameManager
{
private:
	Board chessBoard;
	Team CurrentTeam = Team::WHITE;

	struct Command {
		char Letter;
		int Number;
	};
	std::queue<Command> Commands;
	bool isReplaying = false;

	/*ChessSaveLoadManager fileManager;
	ChessSaveData data;*/

	bool isGameRunning = false;
public:
	~GameManager();
	void Replay();
	void Load(const char* filePath);
	void Save(const char* filePath);
	void Init();
	void Update();
private:
	int LetterToNumber(char letter);
	bool isValidInput(int input);
	bool isValidInput(char input);
	void ProcessInput(int targetX, int targetY, int destX, int destY);
	void Render();
};