#include "Enemy.h"

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
}

int CEnemy::GetAreaRadius()
{
	return areaRadius;
}

bool CEnemy::IsPlayerNearby(float x, float y, float playerX, float playerY, float radius)
{
	if (pow(playerX - x, 2) + pow(playerY - y, 2) <= pow(radius, 2))
	{
		return true;
	}
	
	return false;
}