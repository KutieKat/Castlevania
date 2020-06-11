#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CTripleShot : public CGameObject
{
public:
	CTripleShot();

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};