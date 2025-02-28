#include "Unit.h"
#include "Board.h"

Unit::Unit(std::string name, char symbol, Team t, int x, int y)
: Name(name), Symbol(symbol), team(t), x(x), y(y), bDead(false) {
}

std::string Unit::GetName() const
{
    return Name;
}

int Unit::GetX() const {
    return x;
}

int Unit::GetY() const
{
    return y;
}

bool Unit::IsDead() const
{
    return bDead;
}

char Unit::GetSymbol() const
{
    return Symbol;
}

Team Unit::GetTeam() const
{
    return team;
}

void Unit::SetDead(bool bDead)
{
    this->bDead = bDead;
}

void Unit::Move(int x, int y)
{
    this->x = x; this->y = y;
}

bool Unit::CanMove(int x, int y, class Board*)
{
    return false;
}

/*
	Rook
*/
Rook::Rook(std::string name, char symbol, Team team, int x, int y)
	: Unit(name, symbol, team, x, y)
{

}

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

/*
	Knight
*/
Knight::Knight(std::string name, char symbol, Team team, int x, int y)
	: Unit(name, symbol, team, x, y)
{
}

bool Knight::CanMove(int x, int y, Board* chessBoard)
{
	//x,y는 맵밖인지 이미 검사되어 들어온다. 같은 팀인지 여부도
	
	//위쪽 왼쪽
	if (GetX() - 1 == x && GetY() - 2 == y)
		return true;
	//위쪽 오른쪽
	if (GetX() + 1 == x && GetY() - 2 == y)
		return true;

	//왼쪽 위쪽
	if (GetX() - 2 == x && GetY() - 1 == y)
		return true;
	//왼쪽 아래쪽
	if (GetX() - 2 == x && GetY() + 1 == y)
		return true;

	//오른쪽 위쪽
	if (GetX() + 2 == x && GetY() - 1 == y)
		return true;
	//오른쪽 아래쪽
	if (GetX() + 2 == x && GetY() + 1 == y)
		return true;

	//아래쪽 왼쪽
	if (GetX() + 1 == x && GetY() + 2 == y)
		return true;

	//아래쪽 오른쪽
	if (GetX() - 1 == x && GetY() + 2 == y)
		return true;
	
	//다 못가면 못감
	return false;
}
