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
	// ���� ��ġ�� ��ǥ ��ġ ������ ���� ����
	int dx = (GetX() == x) ? 0 : (GetX() > x) ? -1 : 1;  // x�� ������ ������ dx�� 0
	int dy = (GetY() == y) ? 0 : (GetY() > y) ? -1 : 1;  // y�� ������ ������ dy�� 0

	// ���� ������ �̵� �����ϹǷ�, x�� y �� �� �ϳ��� 0�� �ƴϾ�� �Ѵ�.
	if (dx != 0 && dy != 0) {
		return false;  // ���� �밢������ �̵��� �� ���� ������ dx, dy ��� 0�� �ƴϸ� �̵� �Ұ�
	}

	// tempX, tempY�� ���� ���� ��ġ���� ��ǥ ��ġ������ ��θ� ���� �̵��� ����
	int tempX = GetX() + dx;
	int tempY = GetY() + dy;

	// ��ǥ ��ġ���� �� ĭ�� �̵��ϸ� Ȯ�� �������� �˻�X
	while (tempX != x || tempY != y) {
		Team team = chessBoard->GetGridInfo(tempX, tempY);

		// ��ο� �ٸ� ���� ������ �̵� �Ұ�
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
	//x,y�� �ʹ����� �̹� �˻�Ǿ� ���´�. ���� ������ ���ε�
	
	//���� ����
	if (GetX() - 1 == x && GetY() - 2 == y)
		return true;
	//���� ������
	if (GetX() + 1 == x && GetY() - 2 == y)
		return true;

	//���� ����
	if (GetX() - 2 == x && GetY() - 1 == y)
		return true;
	//���� �Ʒ���
	if (GetX() - 2 == x && GetY() + 1 == y)
		return true;

	//������ ����
	if (GetX() + 2 == x && GetY() - 1 == y)
		return true;
	//������ �Ʒ���
	if (GetX() + 2 == x && GetY() + 1 == y)
		return true;

	//�Ʒ��� ����
	if (GetX() + 1 == x && GetY() + 2 == y)
		return true;

	//�Ʒ��� ������
	if (GetX() - 1 == x && GetY() + 2 == y)
		return true;
	
	//�� ������ ����
	return false;
}
