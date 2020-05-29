#include "EnemySpawnerArea.h"
#include "../../Game.h"
#include "../Characters/Enemies/Ghost.h"
#include "../Unit.h"

CEnemySpawnerArea::CEnemySpawnerArea(CSimon* simon)
{
	this->nextSpawningTime = -1;
	this->spawningInterval = ENEMY_SPAWNER_SPAWNING_INTERVAL;
	this->simon = simon;

	SetAnimationSet("transparency");
}

void CEnemySpawnerArea::Render()
{
	animationSet->at(0)->Render(x, y);
}

void CEnemySpawnerArea::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + ENEMY_SPAWNER_AREA_BBOX_WIDTH;
	bottom = top + ENEMY_SPAWNER_AREA_BBOX_HEIGHT;
}

void CEnemySpawnerArea::SetEnemyType(string type)
{
	enemyType = type;
}

void CEnemySpawnerArea::SetInterval(DWORD interval)
{
	if (interval != -1 && interval != ENEMY_SPAWNER_SPAWNING_INTERVAL)
	{
		spawningInterval = interval;
	}
}

void CEnemySpawnerArea::SetSpawningPosition(string position)
{
	spawningPosition = position;
}

void CEnemySpawnerArea::SpawnEnemy()
{
	if (nextSpawningTime == -1)
	{
		CreateEnemy();
		nextSpawningTime = GetTickCount() + spawningInterval;
	}
	else
	{
		if (GetTickCount() > nextSpawningTime)
		{
			CreateEnemy();
			nextSpawningTime = GetTickCount() + spawningInterval;
		}
	}
}

void CEnemySpawnerArea::CreateEnemy()
{
	if (enemyType == "ghost")
	{
		spawnedEnemy = new CGhost(simon);

		if (spawningPosition == "bottom_right")
		{
			spawnedEnemy->SetPosition(simon->x + 320, simon->y + 20);
			spawnedEnemy->SetDirectionX(Direction::Left);
		}

		CGrid* grid = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->GetGrid();
		CUnit* unit = new CUnit(grid, spawnedEnemy, spawnedEnemy->x, spawnedEnemy->y);
	}
}
