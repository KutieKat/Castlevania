#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CBigCandle : public CGameObject
{
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual CBoundingBox GetBoundingBox();
};