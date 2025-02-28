#pragma once
#include "ChessData.h"
class Unit;
class Rook;

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
	void Init();
	
	bool MoveUnit(int FromX, int FromY, Team Team, int ToX, int ToY);
	

	Unit** GetUnits();

	// "���� �̵��� ��ģ ��" üũ����Ʈ �˻�ϱ� *****��밡 üũ����Ʈ ��������***** �˻��ϴ� �Լ�.
	bool CheckMate(Team currentTeam);

	Team GetGridInfo(int x, int y);
	
private:
	bool CanMove(int x, int y);

	//Attacker�κ��� ������ ������ Team������ �ִ���
	bool CanBlockCheck(int kingX, int kingY, Unit& Attacker, Team Team);

	// Ư�� ��ǥ�� Team ���ֵ鿡 ���� ���� ������� Ȯ���ϴ� �Լ�
	bool CanMoveUnits(int targetX, int targetY, Team Team);


	void FindAttackingUnit(int kingX, int kingY, Team enemyTeam, Unit** attacker);

private:
	Unit** Units;

	//���� ���ְ˻� ������ King�� ��Ƶδ°� ���� �� ����
	Unit* Kings[2] = { nullptr };

	const int UNIT_SIZE = 32;
	const int BOARD_HEIGHT = 8;
	const int BOARD_WIDTH = 8;
	bool bEndGame = false;
};
