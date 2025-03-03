#pragma once
#include <string>

enum class Team : int
{
	NONE = -1,
	BLACK,
	WHITE
};

struct UnitInfo
{
	std::string Name;
	char Symbol;
	int team;
	int x;
	int y;
	bool bDead;
};

struct ChessSaveData
{
	ChessSaveData():units(),currentTeam(-1){}
	int currentTeam;
	UnitInfo* units;
};

