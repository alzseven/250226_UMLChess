#include "Board.h"
#include "Unit.h"
#include "Units.h"

void Board::Init(ChessSaveData data, bool canReadable)
{
	Units = new Unit * [UNIT_SIZE];

	Units[0] = new Rook("Rook", 'R', Team::BLACK, 0, 0);
	Units[1] = new Knight("Knight", 'N', Team::BLACK, 1, 0);
	Units[2] = new Unit("Bishop", 'B', Team::BLACK, 2, 0);
	Units[3] = new Unit("Queen", 'Q', Team::BLACK, 3, 0);
	Units[4] = new Unit("King", 'K', Team::BLACK, 4, 0);
	Units[5] = new Unit("Bishop", 'B', Team::BLACK, 5, 0);
	Units[6] = new Knight("Knight", 'N', Team::BLACK, 6, 0);
	Units[7] = new Rook("Rook", 'R', Team::BLACK, 7, 0);

		for (int i = 8; i < 16; i++)
			Units[i] = new Pawn("Pawn", 'P', Team::BLACK, i - 8, 1);

		for (int i = 16; i < 24; i++)
			Units[i] = new Pawn("Pawn", 'p', Team::WHITE, i - 16, 6);

	Units[24] = new Rook("Rook", 'r', Team::WHITE, 0, 7);
	Units[25] = new Knight("Knight", 'n', Team::WHITE, 1, 7);
	Units[26] = new Unit("Bishop", 'b', Team::WHITE, 2, 7);
	Units[27] = new Unit("Queen", 'q', Team::WHITE, 3, 7);
	Units[28] = new Unit("King", 'k', Team::WHITE, 4, 7);
	Units[29] = new Unit("Bishop", 'b', Team::WHITE, 5, 7);
	Units[30] = new Knight("Knight", 'n', Team::WHITE, 6, 7);
	Units[31] = new Rook("Rook", 'r', Team::WHITE, 7, 7);

	}
	// 킹 위치 추적
	Kings[0] = Units[4]; // 흑 킹
	Kings[1] = Units[28]; // 백 킹
}

bool Board::MoveUnit(int FromX, int FromY, Team Team, int ToX, int ToY)
{
	//맵 밖 입력인지 확인
	if (!CanMove(ToX, ToY))
		return false;

	// 해당 좌표에 적 유닛이 있는지 확인
	int EnemyUnitIndex = -1;
	for (int i = 0; i < UNIT_SIZE; i++)
	{
		if (Units[i]->GetX() == ToX && Units[i]->GetY() == ToY && !Units[i]->IsDead())
		{
			//같은팀을 죽이려고 하는경우
			if (Units[i]->GetTeam() == Team)
				return false;

			EnemyUnitIndex = i;  // 적 유닛이 있는 위치 저장
		}
	}

	// 유닛을 시작 좌표(FromX, FromY)에서 찾음
	int unitIndex = -1;
	for (int i = 0; i < UNIT_SIZE; i++)
	{
		if (Units[i]->GetX() == FromX && Units[i]->GetY() == FromY && !Units[i]->IsDead())
		{
			//현재 팀과 다른유닛을 선택하면 false
			if (Units[i]->GetTeam() != Team)
				return false;

			unitIndex = i;
			break;
		}
	}

	if (unitIndex == -1)
		return false;

	// 해당 유닛이 이동할 수 있는지 확인
	if (!Units[unitIndex]->CanMove(ToX, ToY, this))
		return false;

	// 유닛 이동
	Units[unitIndex]->Move(ToX, ToY);

	if (EnemyUnitIndex >= 0)
	{
		//이동하고자 했던 위치에 적이 있다면 죽임
		Unit* EnemyUnit = Units[EnemyUnitIndex];

		EnemyUnit->SetDead(true);
	}
	return true;
}

Unit** Board::GetUnits()
{
	return Units;
}


// "내가 이동을 마친 후" 체크메이트 검사니까 *****상대가 체크메이트 상태인지***** 검사하는 함수.
bool Board::CheckMate(Team currentTeam)
{
	// 현재 팀과 반대 팀을 알아낸다
	Team oppositeTeam = (currentTeam == Team::WHITE) ? Team::BLACK : Team::WHITE;

	// 1. 반대팀 킹의 위치 찾기
	int kingX = Kings[(int)oppositeTeam]->GetX();
	int kingY = Kings[(int)oppositeTeam]->GetY();

	Unit* Attacker = nullptr;

	// 체크를 유발하는 아군 유닛을 캐싱 (2개이상의 유닛이 동시에 체크를 유발하는일은 없다.)
	FindAttackingUnit(kingX, kingY, currentTeam, &Attacker);

	//공격자가 없다면 체크상태도 아님
	if (Attacker == nullptr)
		return false;

	// 2. 상대 킹이 체크 상태면, 킹을 이동할 수 있는지 확인 (피할 수 있으면 체크메이트 아님)
	// 킹이 이동할 수 있는 8개 칸을 확인
	int dx[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	int dy[] = { -1, -1, -1, 0, 0, 1, 1, 1 };

	for (int i = 0; i < 8; i++)
	{
		int newX = kingX + dx[i];
		int newY = kingY + dy[i];

		//newX,newY가 맵 범위안쪽인지 검사하고, 다른 유닛들이 킹 8칸을 공격할 수 있는지 확인
		//아군 유닛들이 한 칸이라도 킹 주변 8칸을 공격할 수 없다면 체크메이트가 아님
		if (CanMove(newX, newY) && !CanMoveUnits(newX, newY, currentTeam))
			return false;
	}

	// 3. 체크를 유발하는 아군 유닛들을 죽일 상대 유닛이 있는지, 이미 체크라면 무조건 공격자가 있기 때문에 null검사는 안함
	int attackerX = Attacker->GetX();
	int attackerY = Attacker->GetY();

	// 상대 유닛이 아군 공격자를 잡을 수 있는지 확인
	if (CanMoveUnits(attackerX, attackerY, oppositeTeam))
		return false; // 공격자를 잡을 수 있으면 체크메이트 아님

	// 4. 공격자를 잡을 수 없고, 남은수단은 몸빵
	// 상대 유닛이 아군 공격자의 공격경로를 막을수 있는지
	if (!CanBlockCheck(kingX, kingY, *Attacker, oppositeTeam))
		return true;

	return false; // 체크 상태가 아니거나, 방어 가능한 경우
}

Team Board::GetGridInfo(int x, int y)
{
	for (int i = 0; i < UNIT_SIZE; i++)
	{
		if (Units[i]->IsDead())
			continue;

		//좌표에 뭔가 있다면!
		if (Units[i]->GetX() == x && Units[i]->GetY() == y)
			return Units[i]->GetTeam();
	}
	//다돌았는데 암것도 없다면
	return Team::NONE;
}

Team Board::IToTeam(int i)
{
	return i == 0 ? Team::BLACK : i == 1 ? Team::WHITE : Team::NONE;
}

Unit* Board::MapDataWithUnitInstance(const UnitInfo& info)
{
	Unit* unit;
	if (info.Name == "Rook") {
		unit = new Rook(info.Name, info.Symbol, IToTeam(info.team), info.x, info.y);
		unit->SetDead(info.bDead);
	}
	else if (info.Name == "Bishop") {
		unit = new Bishop(info.Name, info.Symbol, IToTeam(info.team), info.x, info.y);
		unit->SetDead(info.bDead);
	}
	else if (info.Name == "Knight") {
		unit = new Knight(info.Name, info.Symbol, IToTeam(info.team), info.x, info.y);
		unit->SetDead(info.bDead);
	}
	else if (info.Name == "Queen") {
		unit = new Queen(info.Name, info.Symbol, IToTeam(info.team), info.x, info.y);
		unit->SetDead(info.bDead);
	}
	else if (info.Name == "King") {
		unit = new King(info.Name, info.Symbol, IToTeam(info.team), info.x, info.y);
		unit->SetDead(info.bDead);
	}
	else if (info.Name == "Pawn") {
		unit = new Pawn(info.Name, info.Symbol, IToTeam(info.team), info.x, info.y);
		unit->SetDead(info.bDead);
	}
	else {
		unit = new Unit("", ' ', Team::NONE, 0, 0);
		std::cerr << "ERROR :: UnitInfo::MapDataWithUnitInstance()" << '\n';
	}
	return unit;
}


bool Board::CanMove(int x, int y)
{
	if (x < 0 || y < 0 || x >= BOARD_WIDTH || y >= BOARD_HEIGHT)
		return false;

	return true;
}


//Attacker로부터 공격을 몸빵할 Team유닛이 있는지
bool Board::CanBlockCheck(int kingX, int kingY, Unit& Attacker, Team Team)
{
	int attackerX = Attacker.GetX();
	int attackerY = Attacker.GetY();

	// 이동 방향 설정 (가로, 세로, 대각선) 대각선이라면 방향이 (1,1) (-1,-1), (1,-1), (-1,1)이 됨
	int dx = (attackerX > kingX) ? 1 : (attackerX < kingX) ? -1 : 0;
	int dy = (attackerY > kingY) ? 1 : (attackerY < kingY) ? -1 : 0;

	// 공격자가 직선 공격이 아니면 차단 불가능 (나이트, 폰은 경로 차단 불가)
	if (dx == 0 && dy == 0) return false;

	// 킹과 공격자 사이의 칸을 탐색
	int x = kingX + dx;
	int y = kingY + dy;

	// 공격자로부터 한칸 한칸씩 확인함.
	while (x != attackerX || y != attackerY)
	{
		// 이 위치로 이동할 수 있는 유닛이 있는지 확인
		if (CanMoveUnits(x, y, Team))
		{
			return true; // 경로를 막을 수 있으면 체크메이트 아님
		}

		x += dx;
		y += dy;
	}

	return false; // 막을 방법 없음 -> 체크메이트
}


// 특정 좌표가 Team 유닛들에 의한 공격 대상인지 확인하는 함수
bool Board::CanMoveUnits(int targetX, int targetY, Team Team)
{
	// 유닛들이 해당 위치를 공격할 수 있는지 확인
	for (int i = 0; i < UNIT_SIZE; i++)
	{
		if (Units[i]->IsDead() || Units[i]->GetTeam() != Team)
			continue;

		if (Units[i]->CanMove(targetX, targetY, this))
		{
			return true; // 공격할 수 있는 유닛이 있음
		}
	}

	return false; // 공격할 수 없는 경우
}

void Board::FindAttackingUnit(int kingX, int kingY, Team enemyTeam, Unit** attacker)
{
	for (int i = 0; i < UNIT_SIZE; i++)
	{
		if (Units[i]->IsDead() || Units[i]->GetTeam() != enemyTeam)
			continue;

		if (Units[i]->CanMove(kingX, kingY, this))
		{
			// 상대 킹의 좌표를 위협하는 유닛을 attacker에 넣음
			*attacker = Units[i];
			break;
		}
	}
}
