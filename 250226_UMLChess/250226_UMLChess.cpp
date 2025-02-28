#include <conio.h>
#include <iostream>
#include <string>

#include "ChessSaveLoadManager.h"
#include "ChessData.h"
#include "Unit.h"

using namespace std;

class GameManager;
class Board;

class Bishop : public Unit {
public:
	Bishop(string name, char symbol, Team team, int x, int y)
		: Unit(name, symbol, team, x, y)
	{

	}

	virtual bool CanMove(int x, int y, class Board* chessBoard) override;
};

class Rook : public Unit
{
public:
	Rook(string name, char symbol, Team team, int x, int y)
		: Unit(name, symbol, team, x, y)
	{

	}

	virtual bool CanMove(int x, int y, class Board* chessBoard) override;
};


class Knight : public Unit {
public:
	Knight(string name, char symbol, Team team, int x, int y)
		: Unit(name, symbol, team, x, y)
	{

	}

	virtual bool CanMove(int x, int y, class Board* chessBoard) override;
};
class Queen : public Unit {
public:
	Queen(string name, char symbol, Team team, int x, int y)
		: Unit(name, symbol, team, x, y)
	{

	}
	virtual bool CanMove(int x, int y, class Board* chessBoard) override;
};
class King : public Unit {
public:
	King(string name, char symbol, Team team, int x, int y)
		: Unit(name, symbol, team, x, y)
	{

	}
	virtual bool CanMove(int x, int y, class Board* chessBoard) override;
};
class Pawn : public Unit {
public:
	Pawn(string name, char symbol, Team team, int x, int y)
		: Unit(name, symbol, team, x, y)
	{

	}
	virtual bool CanMove(int x, int y, class Board* chessBoard) override;
};
class Board
{
public:
	~Board()
	{
		for (int i = 0; i < UNIT_SIZE; i++)
		{
			delete Units[i];
		}
		delete[] Units;
	}
public:
	void Init(ChessSaveData data, bool canReadable)
	{
		Units = new Unit * [UNIT_SIZE];
		
		if (canReadable)
		{
			for (int i=0; i < UNIT_SIZE; ++i)
			{
				Units[i] = MapDataWithUnitInstance(data.units[i]);
			}
		}
		else
		{
			Units[0] = new Rook("Rook", 'R', Team::BLACK, 0, 0);
			Units[1] = new Knight("Knight", 'N', Team::BLACK, 1, 0);
			Units[2] = new Bishop("Bishop", 'B', Team::BLACK, 2, 0);
			Units[3] = new Queen("Queen", 'Q', Team::BLACK, 3, 0);
			Units[4] = new King("King", 'K', Team::BLACK, 4, 0);
			Units[5] = new Bishop("Bishop", 'B', Team::BLACK, 5, 0);
			Units[6] = new Knight("Knight", 'N', Team::BLACK, 6, 0);
			Units[7] = new Rook("Rook", 'R', Team::BLACK, 7, 0);

			for (int i = 8; i < 16; i++)
				Units[i] = new Pawn("Pawn", 'P', Team::BLACK, i - 8, 1);

			for (int i = 16; i < 24; i++)
				Units[i] = new Pawn("Pawn", 'p', Team::WHITE, i - 16, 6);

			Units[24] = new Rook("Rook", 'r', Team::WHITE, 0, 7);
			Units[25] = new Knight("Knight", 'n', Team::WHITE, 1, 7);
			Units[26] = new Bishop("Bishop", 'b', Team::WHITE, 2, 7);
			Units[27] = new Queen("Queen", 'q', Team::WHITE, 3, 7);
			Units[28] = new King("King", 'k', Team::WHITE, 4, 7);
			Units[29] = new Bishop("Bishop", 'b', Team::WHITE, 5, 7);
			Units[30] = new Knight("Knight", 'n', Team::WHITE, 6, 7);
			Units[31] = new Rook("Rook", 'r', Team::WHITE, 7, 7);

		}
		// 킹 위치 추적
		Kings[0] = Units[4]; // 흑 킹
		Kings[1] = Units[28]; // 백 킹
	}
	
	bool MoveUnit(int FromX, int FromY, Team Team, int ToX, int ToY)
	{
		//맵 밖 입력인지 확인
		if (!CanMove(ToX, ToY))
			return false;

		// 해당 좌표에 적 유닛이 있는지 확인
		int EnemyUnitIndex = -1;
		for (int i = 0; i < UNIT_SIZE; i++)
		{
			if (Units[i]->GetX() == ToX && Units[i]->GetY() == ToY && !Units[i]->IsDead())
			{
				//같은팀을 죽이려고 하는경우
				if (Units[i]->GetTeam() == Team)
					return false;

				EnemyUnitIndex = i;  // 적 유닛이 있는 위치 저장
			}
		}

		// 유닛을 시작 좌표(FromX, FromY)에서 찾음
		int unitIndex = -1;
		for (int i = 0; i < UNIT_SIZE; i++)
		{
			if (Units[i]->GetX() == FromX && Units[i]->GetY() == FromY && !Units[i]->IsDead())
			{
				//현재 팀과 다른유닛을 선택하면 false
				if (Units[i]->GetTeam() != Team)
					return false;

				unitIndex = i;
				break;
			}
		}

		if (unitIndex == -1)
			return false;

		// 해당 유닛이 이동할 수 있는지 확인
		if (!Units[unitIndex]->CanMove(ToX, ToY, this))
			return false;

		// 유닛 이동
		Units[unitIndex]->Move(ToX, ToY);

		if (EnemyUnitIndex >= 0)
		{
			//이동하고자 했던 위치에 적이 있다면 죽임
			Unit* EnemyUnit = Units[EnemyUnitIndex];

			EnemyUnit->SetDead(true);
		}
		return true;
	}

	Unit** GetUnits()
	{
		return Units;
	}

	// "내가 이동을 마친 후" 체크메이트 검사니까 *****상대가 체크메이트 상태인지***** 검사하는 함수.
	bool CheckMate(Team currentTeam)
	{
		// 현재 팀과 반대 팀을 알아낸다
		Team oppositeTeam = (currentTeam == Team::WHITE) ? Team::BLACK : Team::WHITE;

		// 1. 반대팀 킹의 위치 찾기
		int kingX = Kings[(int)oppositeTeam]->GetX();
		int kingY = Kings[(int)oppositeTeam]->GetY();

		Unit* Attacker = nullptr;

		// 체크를 유발하는 아군 유닛을 캐싱 (2개이상의 유닛이 동시에 체크를 유발하는일은 없다.)
		FindAttackingUnit(kingX, kingY, currentTeam, &Attacker);

		//공격자가 없다면 체크상태도 아님
		if (Attacker == nullptr)
			return false;

		// 2. 상대 킹이 체크 상태면, 킹을 이동할 수 있는지 확인 (피할 수 있으면 체크메이트 아님)
		// 킹이 이동할 수 있는 8개 칸을 확인
		int dx[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
		int dy[] = { -1, -1, -1, 0, 0, 1, 1, 1 };

		for (int i = 0; i < 8; i++)
		{
			int newX = kingX + dx[i];
			int newY = kingY + dy[i];

			//newX,newY가 맵 범위안쪽인지 검사하고, 다른 유닛들이 킹 8칸을 공격할 수 있는지 확인
			//아군 유닛들이 한 칸이라도 킹 주변 8칸을 공격할 수 없다면 체크메이트가 아님
			if (CanMove(newX, newY) && !CanMoveUnits(newX, newY, currentTeam))
				return false;
		}

		// 3. 체크를 유발하는 아군 유닛들을 죽일 상대 유닛이 있는지, 이미 체크라면 무조건 공격자가 있기 때문에 null검사는 안함
		int attackerX = Attacker->GetX();
		int attackerY = Attacker->GetY();

		// 상대 유닛이 아군 공격자를 잡을 수 있는지 확인
		if (CanMoveUnits(attackerX, attackerY, oppositeTeam))
			return false; // 공격자를 잡을 수 있으면 체크메이트 아님

		// 4. 공격자를 잡을 수 없고, 남은수단은 몸빵
		// 상대 유닛이 아군 공격자의 공격경로를 막을수 있는지
		if (!CanBlockCheck(kingX, kingY, *Attacker, oppositeTeam))
			return true;

		return false; // 체크 상태가 아니거나, 방어 가능한 경우
	}

	Team GetGridInfo(int x, int y)
	{
		for (int i = 0; i < UNIT_SIZE; i++)
		{
			if (Units[i]->IsDead())
				continue;

			//좌표에 뭔가 있다면!
			if (Units[i]->GetX() == x && Units[i]->GetY() == y)
				return Units[i]->GetTeam();
		}
		//다돌았는데 암것도 없다면
		return Team::NONE;
	}
private:
	Team IToTeam(int i)
	{
		return i == 0 ? Team::BLACK : i == 1 ? Team::WHITE : Team::NONE;
	}
	
	Unit* MapDataWithUnitInstance(const UnitInfo& info)
	{
		Unit* unit;
		if (info.Name == "Rook") {
			unit = new Rook(info.Name, info.Symbol, IToTeam(info.team), info.x, info.y);
			unit->SetDead(info.bDead);
		}
		else if(info.Name == "Bishop") {
			unit = new Bishop(info.Name, info.Symbol, IToTeam(info.team), info.x, info.y);
			unit->SetDead(info.bDead);
		}
		else if (info.Name == "Knight") {
			unit = new Knight(info.Name, info.Symbol, IToTeam(info.team), info.x, info.y);
			unit->SetDead(info.bDead);
		}
		else if (info.Name == "Queen") {
			unit = new Queen(info.Name, info.Symbol, IToTeam(info.team), info.x, info.y);
			unit->SetDead(info.bDead);
		}
		else if (info.Name == "King") {
			unit = new King(info.Name, info.Symbol, IToTeam(info.team), info.x, info.y);
			unit->SetDead(info.bDead);
		}
		else if (info.Name == "Pawn") {
			unit = new Pawn(info.Name, info.Symbol, IToTeam(info.team), info.x, info.y);
			unit->SetDead(info.bDead);
		}
		else {
			unit = new Unit("",' ',Team::NONE, 0, 0);
			std::cerr << "ERROR :: UnitInfo::MapDataWithUnitInstance()" << '\n';
		}
		return unit;
	}
	
	
	bool CanMove(int x, int y)
	{
		if (x < 0 || y < 0 || x >= BOARD_WIDTH || y >= BOARD_HEIGHT)
			return false;

		return true;
	}

	//Attacker로부터 공격을 몸빵할 Team유닛이 있는지
	bool CanBlockCheck(int kingX, int kingY, Unit& Attacker, Team Team)
	{
		int attackerX = Attacker.GetX();
		int attackerY = Attacker.GetY();

		// 이동 방향 설정 (가로, 세로, 대각선) 대각선이라면 방향이 (1,1) (-1,-1), (1,-1), (-1,1)이 됨
		int dx = (attackerX > kingX) ? 1 : (attackerX < kingX) ? -1 : 0;
		int dy = (attackerY > kingY) ? 1 : (attackerY < kingY) ? -1 : 0;

		// 공격자가 직선 공격이 아니면 차단 불가능 (나이트, 폰은 경로 차단 불가)
		if (dx == 0 && dy == 0) return false;

		// 킹과 공격자 사이의 칸을 탐색
		int x = kingX + dx;
		int y = kingY + dy;

		// 공격자로부터 한칸 한칸씩 확인함.
		while (x != attackerX || y != attackerY)
		{
			// 이 위치로 이동할 수 있는 유닛이 있는지 확인
			if (CanMoveUnits(x, y, Team))
			{
				return true; // 경로를 막을 수 있으면 체크메이트 아님
			}

			x += dx;
			y += dy;
		}

		return false; // 막을 방법 없음 -> 체크메이트
	}

	// 특정 좌표가 Team 유닛들에 의한 공격 대상인지 확인하는 함수
	bool CanMoveUnits(int targetX, int targetY, Team Team)
	{
		// 유닛들이 해당 위치를 공격할 수 있는지 확인
		for (int i = 0; i < UNIT_SIZE; i++)
		{
			if (Units[i]->IsDead() || Units[i]->GetTeam() != Team)
				continue;

			if (Units[i]->CanMove(targetX, targetY, this))
			{
				return true; // 공격할 수 있는 유닛이 있음
			}
		}

		return false; // 공격할 수 없는 경우
	}

	void FindAttackingUnit(int kingX, int kingY, Team enemyTeam, Unit** attacker)
	{
		for (int i = 0; i < UNIT_SIZE; i++)
		{
			if (Units[i]->IsDead() || Units[i]->GetTeam() != enemyTeam)
				continue;

			if (Units[i]->CanMove(kingX, kingY, this))
			{
				// 상대 킹의 좌표를 위협하는 유닛을 attacker에 넣음
				*attacker = Units[i];
				break;
			}
		}
	}

private:
	Unit** Units;

	//현재 유닛검사 로직상 King은 담아두는게 좋을 것 같다
	Unit* Kings[2] = { nullptr };

	const int UNIT_SIZE = 32;
	const int BOARD_HEIGHT = 8;
	const int BOARD_WIDTH = 8;
	bool bEndGame = false;
};

bool Rook::CanMove(int x, int y, Board* chessBoard)
{
	// 현재 위치와 목표 위치 사이의 방향 설정
	int dx = (GetX() == x) ? 0 : (GetX() > x) ? -1 : 1;  // x는 변하지 않으면 dx는 0
	int dy = (GetY() == y) ? 0 : (GetY() > y) ? -1 : 1;  // y는 변하지 않으면 dy는 0

	// 룩은 직선만 이동 가능하므로, x나 y 둘 중 하나만 0이 아니어야 한다.
	if (dx != 0 && dy != 0) {
		return false;  // 룩은 대각선으로 이동할 수 없기 때문에 dx, dy 모두 0이 아니면 이동 불가
	}

	// tempX, tempY는 룩의 현재 위치에서 목표 위치까지의 경로를 따라 이동할 변수
	int tempX = GetX() + dx;
	int tempY = GetY() + dy;

	// 목표 위치까지 한 칸씩 이동하며 확인 목적지는 검사X
	while (tempX != x || tempY != y) {
		Team team = chessBoard->GetGridInfo(tempX, tempY);

		// 경로에 다른 말이 있으면 이동 불가
		if (team != Team::NONE) {
			return false;
		}

		tempX += dx;
		tempY += dy;
	}

	return true;
}

bool Bishop::CanMove(int x, int y, Board* chessBoard)
{
	// 현재 위치와 목표 위치 사이의 방향 설정
	int dx = (GetX() == x) ? 0 : (GetX() > x) ? -1 : 1;  // x는 변하지 않으면 dx는 0
	int dy = (GetY() == y) ? 0 : (GetY() > y) ? -1 : 1;  // y는 변하지 않으면 dy는 0

	// 룩은 직선만 이동 가능하므로, x나 y 둘 중 하나만 0이 아니어야 한다.
	if (dx == 0 || dy == 0) {
		return false;  // 룩은 대각선으로 이동할 수 없기 때문에 dx, dy 모두 0이 아니면 이동 불가
	}

	// tempX, tempY는 룩의 현재 위치에서 목표 위치까지의 경로를 따라 이동할 변수
	int tempX = GetX() + dx;
	int tempY = GetY() + dy;

	// 목표 위치까지 한 칸씩 이동하며 확인 목적지는 검사X
	while (tempX != x || tempY != y) {
		Team team = chessBoard->GetGridInfo(tempX, tempY);

		// 경로에 다른 말이 있으면 이동 불가
		if (team != Team::NONE) {
			return false;
		}

		tempX += dx;
		tempY += dy;
	}

	return true;
}

bool Queen::CanMove(int x, int y, Board* chessBoard)
{
	// 현재 위치와 목표 위치 사이의 방향 설정
	int dx = (GetX() == x) ? 0 : (GetX() > x) ? -1 : 1;  // x는 변하지 않으면 dx는 0
	int dy = (GetY() == y) ? 0 : (GetY() > y) ? -1 : 1;  // y는 변하지 않으면 dy는 0


	// tempX, tempY는 룩의 현재 위치에서 목표 위치까지의 경로를 따라 이동할 변수
	int tempX = GetX() + dx;
	int tempY = GetY() + dy;

	// 목표 위치까지 한 칸씩 이동하며 확인 목적지는 검사X
	while (tempX != x || tempY != y) {
		Team team = chessBoard->GetGridInfo(tempX, tempY);

		// 경로에 다른 말이 있으면 이동 불가
		if (team != Team::NONE) {
			return false;
		}

		tempX += dx;
		tempY += dy;
	}

	return true;
}

bool King::CanMove(int x, int y, Board* chessBoard)
{
	//TODO:
	return false;
}

bool Knight::CanMove(int x, int y, Board* chessBoard)
{
	//TODO:
	return false;
}

bool Pawn::CanMove(int x, int y, Board* chessBoard)
{
	//TODO:
	return true;
}

class GameManager {
private:
	Board chessBoard;
	Team CurrentTeam = Team::WHITE;

	ChessSaveLoadManager fileManager;
	ChessSaveData data;
	
	bool isGameRunning = false;

public:
	GameManager()
	{
		
	}

	~GameManager()
	{
		Unit** units = chessBoard.GetUnits();
		data.units = new UnitInfo[32];
		for (int i=0; i<32; ++i)
		{
			data.units[i].Name = units[i]->GetName();
			data.units[i].Symbol = units[i]->GetSymbol();
			data.units[i].team = units[i]->GetTeam() == Team::WHITE ? 1 : units[i]->GetTeam() == Team::BLACK ? 0 : -1;
			data.units[i].x = units[i]->GetX();
			data.units[i].y = units[i]->GetY();
		}
		
		if (fileManager.WriteFile("ChessData.txt", data))
		{
			//TODO: Do something...
		}
		else
		{
			std::cerr << "Something wrong happend during save..." << endl;
		}

	}
	
	void Init() {
		bool canReadable = fileManager.ReadFile("ChessData.txt", data);
		chessBoard.Init(data, canReadable);
		isGameRunning = true;
		
		// try {
		// 	fileManager.ReadFile("chess_save.txt", data);
		// }
		


	}



	void Update() {
		Render();
		while (isGameRunning) {
			char targetLetter, destLetter;
			int targetNumber, destNumber;

			cout << (CurrentTeam == Team::WHITE ? "백팀" : "흑팀") << " 플레이어의 턴입니다.\n";

			while (true) {
				targetLetter = ' ';
				cout << "움직이고 싶은 말이 시작하는 알파벳을 입력해주세요.\n";
				cin >> targetLetter;
				if (isValidInput(targetLetter)) {
					break;
				}
				else {
					cout << "잘못된 입력입니다. 다시 입력해주세요.\n";
				}
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
				char symbol = ' ';
				cout << " ";

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