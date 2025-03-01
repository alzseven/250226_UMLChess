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
	// ???? ????? ??? ??? ?????? ???? ????
	int dx = (GetX() == x) ? 0 : (GetX() > x) ? -1 : 1;  // x?? ?????? ?????? dx?? 0
	int dy = (GetY() == y) ? 0 : (GetY() > y) ? -1 : 1;  // y?? ?????? ?????? dy?? 0

	// ???? ?????? ??? ????????, x?? y ?? ?? ????? 0?? ????? ???.
	if (dx != 0 && dy != 0) {
		return false;  // ???? ??O?????? ????? ?? ???? ?????? dx, dy ??? 0?? ???? ??? ???
	}

	// tempX, tempY?? ???? ???? ??????? ??? ????????? ??��? ???? ????? ????
	int tempX = GetX() + dx;
	int tempY = GetY() + dy;

	// ??? ??????? ?? ??? ?????? ??? ???????? ???X
	while (tempX != x || tempY != y) {
		Team team = chessBoard->GetGridInfo(tempX, tempY);

		// ??��? ??? ???? ?????? ??? ???
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
	//x,y?? ??????? ??? ????? ???��?. ???? ?????? ???��?
	
	//???? ????
	if (GetX() - 1 == x && GetY() - 2 == y)
		return true;
	//???? ??????
	if (GetX() + 1 == x && GetY() - 2 == y)
		return true;

	//???? ????
	if (GetX() - 2 == x && GetY() - 1 == y)
		return true;
	//???? ?????
	if (GetX() - 2 == x && GetY() + 1 == y)
		return true;

	//?????? ????
	if (GetX() + 2 == x && GetY() - 1 == y)
		return true;
	//?????? ?????
	if (GetX() + 2 == x && GetY() + 1 == y)
		return true;

	//????? ????
	if (GetX() + 1 == x && GetY() + 2 == y)
		return true;
	//????? ??????
	if (GetX() - 1 == x && GetY() + 2 == y)
		return true;
	
	//?? ?????? ????
	return false;
}


bool Bishop::CanMove(int x, int y, Board* chessBoard)
{
	int distX = GetX() - x;
	int distY = GetY() - y;

	//
	if ((distX != 0 && distY != 0) && (distX / distY == 1 || distX / distY == -1))
	{
		int dx = distX > 1 ? 1 : -1;
		int dy = distY > 1 ? 1 : -1;

		// tempX, tempY?? ???? ???? ??????? ??? ????????? ??��? ???? ????? ????
		int tempX = GetX() + dx;
		int tempY = GetY() + dy;

		// ??? ??????? ?? ??? ?????? ??? ???????? ???X
		while (tempX != x || tempY != y) {
			Team team = chessBoard->GetGridInfo(tempX, tempY);

			// ??��? ??? ???? ?????? ??? ???
			if (team != Team::NONE) {
				return false;
			}

			tempX += dx;
			tempY += dy;
		}

		return true;
	}
	return false;
}


/*
	King
*/
King::King(std::string name, char symbol, Team team, int x, int y)
	: Unit(name, symbol, team, x, y)
{
}

bool King::CanMove(int x, int y, Board* chessBoard)
{
	int dx = abs(x - GetX());
	int dy = abs(y - GetY());

	// ??? ?? ????? ??? ???? (?????��?, ??O??)
	if (dx > 1 || dy > 1)
		return false;

	//King?? ???? "??????" ?????? ????? ??? ????.
	//King?? ???? ????? ??????
	if (chessBoard->GetGridInfo(x,y) == team)
		return false;

	return true;
}
