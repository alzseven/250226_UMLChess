#pragma once
#include "ChessData.h"

class Unit
{
public:
	Unit(std::string name = "", char symbol = '.', Team t = Team::WHITE, int x = 0, int y = 0);

	std::string GetName() const;
	int GetX() const;
	int GetY() const;
	bool IsDead() const;
	char GetSymbol() const;
	Team GetTeam() const;
	void SetDead(bool bDead);
	void Move(int x, int y);
	virtual bool CanMove(int x, int y, class Board*);
protected:
	std::string Name;
	char Symbol;
	Team team;
	int x;
	int y;
	bool bDead;
};

class Rook : public Unit
{
public:
	Rook(std::string name, char symbol, Team team, int x, int y);

	virtual bool CanMove(int x, int y, class Board* chessBoard) override;
};

class Knight : public Unit
{
public:
	Knight(std::string name, char symbol, Team team, int x, int y);

	virtual bool CanMove(int x, int y, class Board* chessBoard) override;
};

class King : public Unit
{
public:
	King(std::string name, char symbol, Team team, int x, int y);

	virtual bool CanMove(int x, int y, class Board* chessBoard) override;
};
