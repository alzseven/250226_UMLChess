#pragma once
#include "ChessData.h"
class Unit;
class Rook;

class Board
{
private:
	Unit** units;
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
	void Init();

	bool MoveUnit(int FromX, int FromY, Team Team, int ToX, int ToY);


	Unit** GetUnits();

	// "내가 이동을 마친 후" 체크메이트 검사니까 *****상대가 체크메이트 상태인지***** 검사하는 함수.
	bool CheckMate(Team currentTeam);

	Team GetGridInfo(int x, int y);

private:
	bool CanMove(int x, int y);

	//Attacker로부터 공격을 몸빵할 Team유닛이 있는지
	bool CanBlockCheck(int kingX, int kingY, Unit& Attacker, Team Team);

	// 특정 좌표가 Team 유닛들에 의한 공격 대상인지 확인하는 함수
	bool CanMoveUnits(int targetX, int targetY, Team Team);


	void FindAttackingUnit(int kingX, int kingY, Team enemyTeam, Unit** attacker);
}