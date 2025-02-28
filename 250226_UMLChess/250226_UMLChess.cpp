
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
	~GameManager()
	{
		if (isReplaying == false)
		{
			Save("Replay.txt");
		}
	}
	void Replay()
	{
		while (isReplaying)
		{
			Command Sour, Dest;

			Sour = Commands.front();
			Commands.pop();

			if (Commands.empty())
			{
				cout << "리플레이 끝" << endl;
				break;
			}
			Dest = Commands.front();
			Commands.pop();

			ProcessInput(LetterToNumber(Sour.Letter), Sour.Number - 1, LetterToNumber(Dest.Letter), Dest.Number - 1);
			system("cls");
			Render();

			Sleep(3000);
			if (Commands.empty())
			{
				cout << "리플레이 끝" << endl;
				break;
			}
		}
	}
	void Load(const char* filePath)
	{
		std::ifstream inFile(filePath);
		if (!inFile)
		{
			std::cerr << "파일을 열 수 없습니다." << filePath << std::endl;
			return;
		}

		while (!inFile.eof())
		{
			Command Cmd;
			inFile >> Cmd.Letter;
			inFile >> Cmd.Number;
			Commands.push(Cmd);
		}

		inFile.close();
	}
	void Save(const char* filePath)
	{
		std::ofstream outFile(filePath);
		if (!outFile)
		{
			std::cerr << "파일을 열 수 없습니다." << filePath << std::endl;
			return;
		}

		while (!Commands.empty())
		{
			outFile << Commands.front().Letter << "\n";
			outFile << Commands.front().Number << "\n";
			Commands.pop();
		}

		outFile.close();
	}

	void Init() {

		chessBoard.Init();
		Render();

		isGameRunning = true;

		cout << "1. 리플레이, 2. 그냥플레이 " << endl;
		int Input;
		cin >> Input;
		if (Input == 1)
		{
			isReplaying = true;
			Load("Replay.txt");
		}
		else
			isReplaying = false;
	}

	void Update() {
		Replay();

		while (isGameRunning && isReplaying == false) {
			char targetLetter, destLetter;
			int targetNumber, destNumber;

			cout << (CurrentTeam == Team::WHITE ? "백팀" : "흑팀") << " 플레이어의 턴입니다.\n";

			while (true) {
				targetLetter = ' ';
				cout << "움직이고 싶은 말이 시작하는 알파벳을 입력해주세요.\n";
				cin >> targetLetter;
				if (targetLetter == 'z')
				{
					isGameRunning = false;
					break;
				}
				if (isValidInput(targetLetter)) {
					break;
				}
				else {
					cout << "잘못된 입력입니다. 다시 입력해주세요.\n";
				}
			}

			if (isGameRunning == false)
			{
				break;
			}

			while (true) {
				targetNumber = -1;
				cout << "움직이고 싶은 말의 번호를 입력해주세요.\n";
				cin >> targetNumber;
				if (isValidInput(targetNumber)) {
					break;
				}
				else {
					cout << "잘못된 입력입니다. 다시 입력해주세요.\n";
				}
			}

			while (true) {
				destLetter = ' ';
				cout << "움직이고 싶은 칸이 존재하는 알파벳을 입력해주세요.\n";
				cin >> destLetter;
				if (isValidInput(destLetter)) {
					break;
				}
				else {
					cout << "잘못된 입력입니다. 다시 입력해주세요.\n";
				}
			}

			while (true) {
				destNumber = -1;
				cout << "움직이고 싶은 칸이 존재하는 숫자를 입력해주세요.\n";
				cin >> destNumber;
				if (isValidInput(destNumber)) {
					break;
				}
				else {
					cout << "잘못된 입력입니다. 다시입력해주세요.\n";
				}
			}

			ProcessInput(LetterToNumber(targetLetter), targetNumber - 1, LetterToNumber(destLetter), destNumber - 1);
			system("cls");
			Render();

		}
	}

private:
	int LetterToNumber(char letter) {
		int result;
		switch (letter) {
		case 'a':
			result = 0;
			break;
		case 'b':
			result = 1;
			break;
		case 'c':
			result = 2;
			break;
		case 'd':
			result = 3;
			break;
		case 'e':
			result = 4;
			break;
		case 'f':
			result = 5;
			break;
		case 'g':
			result = 6;
			break;
		case 'h':
			result = 7;
			break;
		default:
			//TODO:
			result = -1;
			break;
		}
		return result;
	}

	bool isValidInput(int input) {
		bool result;
		switch (input) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
			result = true;
			break;
		default:
			result = false;
			break;
		}
		return result;
	}

	bool isValidInput(char input) {
		bool result;
		switch (input) {
		case 'a':
		case 'b':
		case 'c':
		case 'd':
		case 'e':
		case 'f':
		case 'g':
		case 'h':
			result = true;
			break;
		default:
			result = false;
			break;
		}
		return result;
	}


	void ProcessInput(int targetX, int targetY, int destX, int destY)
	{
		bool result = chessBoard.MoveUnit(targetX, targetY, CurrentTeam, destX, destY);
		if (result == false) {
			// Input values are valid, but cannot move to destination...
			cout << "해당 좌표로 이동할수 없습니다. 다시 입력해주세요. 아무키나 입력";
			_getch();
		}
		else {
			//TODO: Show result when something happened...

			if (isReplaying == false)
			{
				Command Cmd = { targetX + 'a', targetY + 1 };
				Commands.push(Cmd);
				Cmd.Letter = destX + 'a';
				Cmd.Number = destY + 1;
				Commands.push(Cmd);
			}

			//정상적으로 MoveUnit이 호출되었다면
			//상대가 체크메이트 상태인지 검사. 상대검사인데 매개변수로 currentTeam은 모호하다
			bool bCheckMate = chessBoard.CheckMate(CurrentTeam);
			if (bCheckMate)
			{
				cout << "체크메이트! " << (CurrentTeam == Team::WHITE ? "백팀" : "흑팀") << "의 승리입니다!" << endl;
				cout << "아무키나 누르면 종료됩니다" << endl;
				isGameRunning = false;
				_getch();
			}

			//Change team when input is valid
			CurrentTeam = CurrentTeam == Team::WHITE ? Team::BLACK : Team::WHITE;
		}
	}

	void Render() {
		//https://github.com/Sundwalltanner/Ascii-Chess/blob/master/board.cpp

		const int UNIT_SIZE = 32;
		const int BOARD_HEIGHT = 8;
		const int BOARD_WIDTH = 8;

		// Print the top border.
		cout << "  _______________________________________" << endl;

		// Print everything but the bottom letter coords.
		for (int y = BOARD_HEIGHT - 1; y >= 0; y--)
		{
			// Print the upper empty space of the squares and the number coordinate associated with that row.
			cout << " |    |    |    |    |    |    |    |    |\n"
				<< y + 1 << "|";

			// Print the names of the pieces in the squares of this row.
			for (int x = 0; x < BOARD_WIDTH; x++)
			{
				char symbol = '  ';
				cout << symbol;

				Unit** units = chessBoard.GetUnits();
				for (int i = 0; i < UNIT_SIZE; i++)
				{
					if (units[i]->GetX() == x && units[i]->GetY() == y && !units[i]->IsDead())
					{
						symbol = units[i]->GetSymbol();
						break;
					}
				}
				cout << symbol;
				cout << "  |";
			}

			// Print the bottom portion of each square.
			cout << "\n"
				<< " |____|____|____|____|____|____|____|____|" << endl;
		}

		// Print the bottom letter coordinates.
		cout << "   a    b    c    d    e    f    g    h" << endl;
	}
};


int main() {
	GameManager GameMgr;
	GameMgr.Init();
	GameMgr.Update();

	return 0;
}