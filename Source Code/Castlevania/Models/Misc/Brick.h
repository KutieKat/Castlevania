#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CBrick : public CGameObject
{
public:
	CBrick();

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	bool isGround;
};