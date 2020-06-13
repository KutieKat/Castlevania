#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CCursor : public CGameObject
{
public:
	CCursor();

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};