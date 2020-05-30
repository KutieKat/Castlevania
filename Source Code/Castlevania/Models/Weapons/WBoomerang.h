#pragma once
#include "../GameObject.h"
#include "../Characters/Players/Simon.h"

class WBoomerang : public CGameObject
{
public:
	WBoomerang(CSimon* simon);

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Render();

protected:
	CSimon* simon;

	int maxRight;
	int maxLeft;
	int collisionCount;

	bool hidden;
};

