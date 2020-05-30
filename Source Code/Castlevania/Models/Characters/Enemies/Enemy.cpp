#include "Enemy.h"
#include "../../../Game.h"

CEnemy::CEnemy()
{
	mustInArea = false;
	attacks = ENEMY_DEFAULT_ATTACKS;
	delayTimeout = -1;
}

void CEnemy::TakeDamage(int damages)
{
	attacks -= damages;
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