#pragma once
#include "../../GameObject.h"
#include "../../../Utilities/Constants.h"

class CSimon : public CGameObject
{
public:
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void SetState(int state);

	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	CBoundingBox GetBoundingBox();

	bool sitting = false;
	bool jumping = false;
	bool attacking = false;
};