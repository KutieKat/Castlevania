#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CBottomStair : public CGameObject
{
public:
	CBottomStair();

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	Direction directionX;
	Direction directionY;
};