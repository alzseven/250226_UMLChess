#pragma once
#pragma warning(disable:4996)
#include <conio.h>
#include <iostream>
#include <queue>
#include <fstream>
#include <windows.h>
#include "Unit.h"
#include "Board.h"
#define _CRT_SECURE_NO_WARNINGS

using namespace std;

class GameManager {
private:
	Board chessBoard;
	Team CurrentTeam = Team::WHITE;
	bool isGameRunning = false;
	struct Command {
		char Letter;
		int Number;
	};
	queue<Command> Commands;
	bool isReplaying = false;
public:
	//TODO: Save on Console exit(WinAPI)
	//TODO: OR save on end of the game
	/*~GameManager()
	{
		cout << "Out" << endl;
		if (isReplaying == false)
		{
			Save("Replay.txt");
		}
	}*/
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