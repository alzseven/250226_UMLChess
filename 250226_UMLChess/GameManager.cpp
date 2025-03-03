#include "GameManager.h"

void GameManager::Replay()
{
	while (isReplaying)
	{
		Command Sour, Dest;

		Sour = Commands.front();
		Commands.pop();

		if (Commands.empty())
		{
			cout << "���÷��� ��" << endl;
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
			cout << "���÷��� ��" << endl;
			break;
		}
	}
}

void GameManager::Load(const char* filePath)
{
	std::ifstream inFile(filePath);
	if (!inFile)
	{
		std::cerr << "������ �� �� �����ϴ�." << filePath << std::endl;
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

void GameManager::Save(const char* filePath)
{
	std::ofstream outFile(filePath);
	if (!outFile)
	{
		std::cerr << "������ �� �� �����ϴ�." << filePath << std::endl;
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

void GameManager::Init() {

	chessBoard.Init();
	Render();

	isGameRunning = true;

	cout << "1. ���÷���, 2. �׳��÷��� " << endl;
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

void GameManager::Update() {
	Replay();

	while (isGameRunning && isReplaying == false) {
		char targetLetter, destLetter;
		int targetNumber, destNumber;

		cout << (CurrentTeam == Team::WHITE ? "����" : "����") << " �÷��̾��� ���Դϴ�.\n";

		while (true) {
			targetLetter = ' ';
			cout << "�����̰� ���� ���� �����ϴ� ���ĺ��� �Է����ּ���.\n";
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
				cout << "�߸��� �Է��Դϴ�. �ٽ� �Է����ּ���.\n";
			}
		}

		if (isGameRunning == false)
		{
			break;
		}

		while (true) {
			targetNumber = -1;
			cout << "�����̰� ���� ���� ��ȣ�� �Է����ּ���.\n";
			cin >> targetNumber;
			if (isValidInput(targetNumber)) {
				break;
			}
			else {
				cout << "�߸��� �Է��Դϴ�. �ٽ� �Է����ּ���.\n";
			}
		}

		while (true) {
			destLetter = ' ';
			cout << "�����̰� ���� ĭ�� �����ϴ� ���ĺ��� �Է����ּ���.\n";
			cin >> destLetter;
			if (isValidInput(destLetter)) {
				break;
			}
			else {
				cout << "�߸��� �Է��Դϴ�. �ٽ� �Է����ּ���.\n";
			}
		}

		while (true) {
			destNumber = -1;
			cout << "�����̰� ���� ĭ�� �����ϴ� ���ڸ� �Է����ּ���.\n";
			cin >> destNumber;
			if (isValidInput(destNumber)) {
				break;
			}
			else {
				cout << "�߸��� �Է��Դϴ�. �ٽ��Է����ּ���.\n";
			}
		}

		ProcessInput(LetterToNumber(targetLetter), targetNumber - 1, LetterToNumber(destLetter), destNumber - 1);
		system("cls");
		Render();

	}
	if (isReplaying == false)
	{
		Save("Replay.txt");
	}
}

int GameManager::LetterToNumber(char letter) {
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

bool GameManager::isValidInput(int input) {
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

bool GameManager::isValidInput(char input) {
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

void GameManager::ProcessInput(int targetX, int targetY, int destX, int destY)
{
	bool result = chessBoard.MoveUnit(targetX, targetY, CurrentTeam, destX, destY);
	if (result == false) {
		// Input values are valid, but cannot move to destination...
		cout << "�ش� ��ǥ�� �̵��Ҽ� �����ϴ�. �ٽ� �Է����ּ���. �ƹ�Ű�� �Է�";
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

		//���������� MoveUnit�� ȣ��Ǿ��ٸ�
		//��밡 üũ����Ʈ �������� �˻�. ���˻��ε� �Ű������� currentTeam�� ��ȣ�ϴ�
		bool bCheckMate = chessBoard.CheckMate(CurrentTeam);
		if (bCheckMate)
		{
			cout << "üũ����Ʈ! " << (CurrentTeam == Team::WHITE ? "����" : "����") << "�� �¸��Դϴ�!" << endl;
			cout << "�ƹ�Ű�� ������ ����˴ϴ�" << endl;
			isGameRunning = false;
			_getch();
		}

		//Change team when input is valid
		CurrentTeam = CurrentTeam == Team::WHITE ? Team::BLACK : Team::WHITE;
	}
}

void GameManager::Render() {
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
