#pragma once
#include "../../GameObject.h"

class CEnemy : public CGameObject
{
public:
	CEnemy();

	virtual void TakeDamage(int damages = ENEMY_DEFAULT_DAMAGES);
	virtual void OnPlayerEnterArea();

	int GetAreaRadiusX();
	int GetAreaRadiusY();

	static bool IsPlayerNearby(float x, float y, float playerX, float playerY, float radiusX, float radiusY);

protected:
	int attacks;

	int areaRadiusX;
	int areaRadiusY;

	bool mustInArea;
	DWORD delayTimeout;
};

