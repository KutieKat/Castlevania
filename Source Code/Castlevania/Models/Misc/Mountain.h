#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CMountain : public CGameObject
{
public:
	CMountain();

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};