#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CDoubleShot : public CGameObject
{
public:
	CDoubleShot();

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};