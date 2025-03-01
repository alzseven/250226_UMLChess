#include "Board.h"
#include "Unit.h"

void Board::Init()
{
	Units = new Unit * [UNIT_SIZE];

	Units[0] = new Rook("Rook", 'R', Team::BLACK, 0, 0);
	Units[1] = new Knight("Knight", 'N', Team::BLACK, 1, 0);
	Units[2] = new Unit("Bishop", 'B', Team::BLACK, 2, 0);
	Units[3] = new Unit("Queen", 'Q', Team::BLACK, 3, 0);
	Units[4] = new King("King", 'K', Team::BLACK, 4, 0);
	Units[5] = new Unit("Bishop", 'B', Team::BLACK, 5, 0);
	Units[6] = new Knight("Knight", 'N', Team::BLACK, 6, 0);
	Units[7] = new Rook("Rook", 'R', Team::BLACK, 7, 0);

	for (int i = 8; i < 16; i++)
		Units[i] = new Unit("Pawn", 'P', Team::BLACK, i - 8, 1);

	for (int i = 16; i < 24; i++)
		Units[i] = new Unit("Pawn", 'p', Team::WHITE, i - 16, 6);

	Units[24] = new Rook("Rook", 'r', Team::WHITE, 0, 7);
	Units[25] = new Knight("Knight", 'n', Team::WHITE, 1, 7);
	Units[26] = new Unit("Bishop", 'b', Team::WHITE, 2, 7);
	Units[27] = new Unit("Queen", 'q', Team::WHITE, 3, 7);
	Units[28] = new King("King", 'k', Team::WHITE, 4, 7);
	Units[29] = new Unit("Bishop", 'b', Team::WHITE, 5, 7);
	Units[30] = new Knight("Knight", 'n', Team::WHITE, 6, 7);
	Units[31] = new Rook("Rook", 'r', Team::WHITE, 7, 7);

	// 킹 위치 추적
	Kings[0] = Units[4]; // 흑 킹
	Kings[1] = Units[28]; // 백 킹
}

bool Board::MoveUnit(int FromX, int FromY, Team Team, int ToX, int ToY)
{
	//�� �� �Է����� Ȯ��
	if (!CanMove(ToX, ToY))
		return false;

	// �ش� ��ǥ�� �� ������ �ִ��� Ȯ��
	int EnemyUnitIndex = -1;
	for (int i = 0; i < UNIT_SIZE; i++)
	{
		if (Units[i]->GetX() == ToX && Units[i]->GetY() == ToY && !Units[i]->IsDead())
		{
			//�������� ���̷��� �ϴ°��
			if (Units[i]->GetTeam() == Team)
				return false;

			EnemyUnitIndex = i;  // �� ������ �ִ� ��ġ ����
		}
	}

	// ������ ���� ��ǥ(FromX, FromY)���� ã��
	int unitIndex = -1;
	for (int i = 0; i < UNIT_SIZE; i++)
	{
		if (Units[i]->GetX() == FromX && Units[i]->GetY() == FromY && !Units[i]->IsDead())
		{
			//���� ���� �ٸ������� �����ϸ� false
			if (Units[i]->GetTeam() != Team)
				return false;

			unitIndex = i;
			break;
		}
	}

	if (unitIndex == -1)
		return false;

	// �ش� ������ �̵��� �� �ִ��� Ȯ��
	if (!Units[unitIndex]->CanMove(ToX, ToY, this))
		return false;

	// ���� �̵�
	Units[unitIndex]->Move(ToX, ToY);

	if (EnemyUnitIndex >= 0)
	{
		//�̵��ϰ��� �ߴ� ��ġ�� ���� �ִٸ� ����
		Unit* EnemyUnit = Units[EnemyUnitIndex];

		EnemyUnit->SetDead(true);
	}
	return true;
}

Unit** Board::GetUnits()
{
	return Units;
}


// "���� �̵��� ��ģ ��" üũ����Ʈ �˻�ϱ� *****��밡 üũ����Ʈ ��������***** �˻��ϴ� �Լ�.
bool Board::CheckMate(Team currentTeam)
{
	// ���� ���� �ݴ� ���� �˾Ƴ���
	Team oppositeTeam = (currentTeam == Team::WHITE) ? Team::BLACK : Team::WHITE;
	int KingIndex = (int)oppositeTeam;
	
	// 1. �ݴ��� ŷ�� ��ġ ã��
	int kingX = Kings[KingIndex]->GetX();
	int kingY = Kings[KingIndex]->GetY();

	Unit* Attacker = nullptr;

	// üũ�� �����ϴ� �Ʊ� ������ ĳ�� (2���̻��� ������ ���ÿ� üũ�� �����ϴ����� ����.)
	FindAttackingUnit(kingX, kingY, currentTeam, &Attacker);

	//�����ڰ� ���ٸ� üũ���µ� �ƴ�
	if (Attacker == nullptr)
		return false;

	// 2. ��� ŷ�� üũ ���¸�, ŷ�� �̵��� �� �ִ��� Ȯ�� (���� �� ������ üũ����Ʈ �ƴ�)
	// ŷ�� �̵��� �� �ִ� 8�� ĭ�� Ȯ��
	int dx[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	int dy[] = { -1, -1, -1, 0, 0, 1, 1, 1 };

	for (int i = 0; i < 8; i++)
	{
		int newX = kingX + dx[i];
		int newY = kingY + dy[i];

		//newX,newY�� �� ������������ �˻��ϰ�
		if (!CanMove(newX, newY))
			continue;

		//ŷ�� �ֺ����� �̵� �� �� �ְ�
		if (Kings[KingIndex]->CanMove(newX, newY, this))
		{
			//�ٸ� ���ֵ��� ŷ�� �̵��� �� �ִ°��� ������ �� ���ٸ� üũ����Ʈ�� �ƴ�
			if (!CanMoveUnits(newX, newY, currentTeam))
				return false;
		}
	}

	// 3. üũ�� �����ϴ� �Ʊ� ���ֵ��� ���� ��� ������ �ִ���, �̹� üũ��� ������ �����ڰ� �ֱ� ������ null�˻�� ����
	int attackerX = Attacker->GetX();
	int attackerY = Attacker->GetY();

	// ��� ������ �Ʊ� �����ڸ� ���� �� �ִ��� Ȯ��
	if (CanMoveUnits(attackerX, attackerY, oppositeTeam))
		return false; // �����ڸ� ���� �� ������ üũ����Ʈ �ƴ�

	// 4. �����ڸ� ���� �� ����, ���������� ����
	// ��� ������ �Ʊ� �������� ���ݰ�θ� ������ �ִ���
	if (!CanBlockCheck(kingX, kingY, *Attacker, oppositeTeam))
		return true;

	return false; // üũ ���°� �ƴϰų�, ��� ������ ���
}

Team Board::GetGridInfo(int x, int y)
{
	for (int i = 0; i < UNIT_SIZE; i++)
	{
		if (Units[i]->IsDead())
			continue;

		//��ǥ�� ���� �ִٸ�!
		if (Units[i]->GetX() == x && Units[i]->GetY() == y)
			return Units[i]->GetTeam();
	}
	//�ٵ��Ҵµ� �ϰ͵� ���ٸ�
	return Team::NONE;
}


bool Board::CanMove(int x, int y)
{
	if (x < 0 || y < 0 || x >= BOARD_WIDTH || y >= BOARD_HEIGHT)
		return false;

	return true;
}


//Attacker�κ��� ������ ������ Team������ �ִ���
bool Board::CanBlockCheck(int kingX, int kingY, Unit& Attacker, Team Team)
{
	if (Attacker.GetName() == "Knight" || Attacker.GetName() == "Pawn")
		return false;

	int attackerX = Attacker.GetX();
	int attackerY = Attacker.GetY();

	// �̵� ���� ���� (����, ����, �밢��) �밢���̶�� ������ (1,1) (-1,-1), (1,-1), (-1,1)�� ��
	int dx = (attackerX > kingX) ? 1 : (attackerX < kingX) ? -1 : 0;
	int dy = (attackerY > kingY) ? 1 : (attackerY < kingY) ? -1 : 0;
	
	// �����ڰ� ���� ������ �ƴϸ� ���� �Ұ��� (����Ʈ, ���� ��� ���� �Ұ�)
	if (dx == 0 && dy == 0) return false;
	
	// ŷ�� ������ ������ ĭ�� Ž��
	int x = kingX + dx;
	int y = kingY + dy;

	// �����ڷκ��� ��ĭ ��ĭ�� Ȯ����.
	while (x != attackerX || y != attackerY)
	{
		// �� ��ġ�� �̵��� �� �ִ� ������ �ִ��� Ȯ��
		if (CanMoveUnits(x, y, Team))
		{
			return true; // ��θ� ���� �� ������ üũ����Ʈ �ƴ�
		}

		x += dx;
		y += dy;
	}

	return false; // ���� ��� ���� -> üũ����Ʈ
}


// Ư�� ��ǥ�� Team ���ֵ鿡 ���� ���� ������� Ȯ���ϴ� �Լ�
bool Board::CanMoveUnits(int targetX, int targetY, Team Team)
{
	// ���ֵ��� �ش� ��ġ�� ������ �� �ִ��� Ȯ��
	for (int i = 0; i < UNIT_SIZE; i++)
	{
		if (Units[i]->IsDead() || Units[i]->GetTeam() != Team)
			continue;

		if (Units[i]->CanMove(targetX, targetY, this))
		{
			return true; // ������ �� �ִ� ������ ����
		}
	}

	return false; // ������ �� ���� ���
}

void Board::FindAttackingUnit(int kingX, int kingY, Team enemyTeam, Unit** attacker)
{
	for (int i = 0; i < UNIT_SIZE; i++)
	{
		if (Units[i]->IsDead() || Units[i]->GetTeam() != enemyTeam)
			continue;

		if (Units[i]->CanMove(kingX, kingY, this))
		{
			// ��� ŷ�� ��ǥ�� �����ϴ� ������ attacker�� ����
			*attacker = Units[i];
			break;
		}
	}
}
