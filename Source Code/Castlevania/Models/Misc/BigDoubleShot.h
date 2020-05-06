#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CBigDoubleShot : public CGameObject
{
public:
	CBigDoubleShot();

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};