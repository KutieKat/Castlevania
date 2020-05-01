#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CTopStair : public CGameObject
{
public:
	CTopStair();

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	Direction directionX;
	Direction directionY;
};