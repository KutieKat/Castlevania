#include "Enemy.h"
#include "../../../Game.h"
#include "../../Effects/Flash.h"

CEnemy::CEnemy()
{
	mustInArea = false;
	attacks = ENEMY_DEFAULT_ATTACKS;
	delayTimeout = -1;
	elevation = ENEMY_DEFAULT_ELEVATION;
}

void CEnemy::TakeDamage(int damages)
{
	attacks -= damages;

	if (endingEffect == nullptr)
	{
		endingEffect = new CFlash();
		endingEffect->SetPosition(x, y);

		CGrid* grid = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->GetGrid();
		CUnit* unit = new CUnit(grid, endingEffect, x, y);
	}
}

void CEnemy::OnPlayerEnterArea()
{
	bool softPaused = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->SoftPaused();

	if (softPaused) return;
}

int CEnemy::GetAreaRadiusX()
{
	return areaRadiusX;
}

int CEnemy::GetAreaRadiusY()
{
	return areaRadiusY;
}

bool CEnemy::IsPlayerNearby(float x, float y, float playerX, float playerY, float radiusX, float radiusY)
{
	if (playerX >= x - radiusX && playerX <= x + radiusX && playerY >= y - radiusY && playerY <= y + radiusY)
	{
		return true;
	}
	
	return false;
}