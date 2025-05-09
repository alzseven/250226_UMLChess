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
	ChessSaveData():currentTeam(-1),units(nullptr){}
	int currentTeam;
	UnitInfo* units;
};

