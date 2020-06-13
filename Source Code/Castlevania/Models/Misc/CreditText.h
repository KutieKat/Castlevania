#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CCreditText : public CGameObject
{
public:
	CCreditText();

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};