#pragma once
#include "Unit.h"

class Bishop : public Unit {
public:
	Bishop(std::string name, char symbol, Team team, int x, int y)
		: Unit(name, symbol, team, x, y)
	{

	}

	bool CanMove(int x, int y, class Board* chessBoard) {

	}
};

class Rook : public Unit
{
public:
	Rook(std::string name, char symbol, Team team, int x, int y)
		: Unit(name, symbol, team, x, y)
	{

	}

	virtual bool CanMove(int x, int y, class Board* chessBoard) override;
};


class Knight : public Unit {
public:
	Knight(std::string name, char symbol, Team team, int x, int y)
		: Unit(name, symbol, team, x, y)
	{

	}

	virtual bool CanMove(int x, int y, class Board* chessBoard) override;
};
class Queen : public Unit {
public:
	Queen(std::string name, char symbol, Team team, int x, int y)
		: Unit(name, symbol, team, x, y)
	{

	}
	virtual bool CanMove(int x, int y, class Board* chessBoard) override;
};
class King : public Unit {
public:
	King(std::string name, char symbol, Team team, int x, int y)
		: Unit(name, symbol, team, x, y)
	{

	}
	virtual bool CanMove(int x, int y, class Board* chessBoard) override;
};
class Pawn : public Unit {
public:
	Pawn(std::string name, char symbol, Team team, int x, int y)
		: Unit(name, symbol, team, x, y)
	{

	}
	virtual bool CanMove(int x, int y, class Board* chessBoard) override;
};