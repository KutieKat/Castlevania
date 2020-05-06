#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CCrown: public CGameObject
{
public:
	CCrown();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void SetState(int state);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};