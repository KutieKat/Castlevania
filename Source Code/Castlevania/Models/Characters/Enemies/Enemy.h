#pragma once
#include "../../GameObject.h"

class CEnemy : public CGameObject
{
public:
	CEnemy();

	virtual void TakeDamage(int damages = ENEMY_DEFAULT_DAMAGES);
	virtual void OnPlayerEnterArea();

	int GetAreaRadius();

	static bool IsPlayerNearby(float x, float y, float playerX, float playerY, float radius);

protected:
	int attacks;
	int areaRadius;

	bool mustInArea;
	DWORD delayTimeout;
};

