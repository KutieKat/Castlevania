#pragma once
#include "../../GameObject.h"
#include "../../../Utilities/Constants.h"

class CRedBat : public CGameObject
{
public:
	CRedBat();

	void SetState(int state);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);

	bool IsPlayerNearby(float playerX, float playerY, float radius);
};