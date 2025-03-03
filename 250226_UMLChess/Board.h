#pragma once
#include "ChessData.h"
#include "Unit.h"

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
	void Init(ChessSaveData data = ChessSaveData());
	
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

	Unit* MapDataWithUnitInstance(const UnitInfo& info);
private:
	Unit** Units;

	//현재 유닛검사 로직상 King은 담아두는게 좋을 것 같다
	Unit* Kings[2] = { nullptr };

	const int UNIT_SIZE = 32;
	const int BOARD_HEIGHT = 8;
	const int BOARD_WIDTH = 8;
	bool bEndGame = false;
};