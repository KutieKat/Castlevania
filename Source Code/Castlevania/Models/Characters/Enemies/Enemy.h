#pragma once
#include "../../GameObject.h"

class CEnemy : public CGameObject
{
public:
	CEnemy();

	virtual void TakeDamage(int damages);
	virtual void OnPlayerEnterArea();

	int GetAreaRadiusX();
	int GetAreaRadiusY();
	int GetDamages();

	static bool IsPlayerNearby(float x, float y, float playerX, float playerY, float radiusX, float radiusY);

protected:
	int attacks;

	int areaRadiusX;
	int areaRadiusY;

	bool mustInArea;
	DWORD delayTimeout;
};

