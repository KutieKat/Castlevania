#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CBackground : public CGameObject
{
public:
	CBackground();

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};