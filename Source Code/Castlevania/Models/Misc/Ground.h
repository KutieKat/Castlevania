#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CGround : public CGameObject
{
public:
	CGround();

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};