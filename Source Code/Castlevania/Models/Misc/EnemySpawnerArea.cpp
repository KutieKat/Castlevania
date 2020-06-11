#include "EnemySpawnerArea.h"
#include "../../Game.h"
#include "../Characters/Enemies/Ghost.h"
#include "../Characters/Enemies/Ghoul.h"
#include "../Effects/Flash.h"
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
	CCamera* camera = CGame::GetInstance()->GetCamera();
	CGrid* grid = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->GetGrid();

	float cameraRight = camera->GetRight();
	float cameraLeft = camera->GetLeft();
	float cameraTop = camera->GetTop();
	float cameraBottom = camera->GetBottom();

	if (enemyType == "ghost")
	{
		spawnedEnemy = new CGhost(simon);
	}
	else if (enemyType == "ghoul")
	{
		spawnedEnemy = new CGhoul(simon);
	}

	if (spawningPosition == "top_right")
	{
		spawnedEnemy->SetPosition(cameraRight - 10, cameraTop + 150);
		spawnedEnemy->SetDirectionX(Direction::Left);
	}
	else if (spawningPosition == "bottom_right")
	{
		spawnedEnemy->SetPosition(cameraRight - 10, enemyType == "ghost" ? simon->y + 20 : cameraBottom - 200);
		spawnedEnemy->SetDirectionX(Direction::Left);
	}
	else if (spawningPosition == "top_left")
	{
		spawnedEnemy->SetPosition(cameraLeft + 10, cameraTop + 150);
		spawnedEnemy->SetDirectionX(Direction::Right);
	}
	else if (spawningPosition == "bottom_left")
	{
		spawnedEnemy->SetPosition(cameraLeft + 10, cameraBottom - 200);
		spawnedEnemy->SetDirectionX(Direction::Right);
	}

	CUnit* unit = new CUnit(grid, spawnedEnemy, spawnedEnemy->x, spawnedEnemy->y);
}
