#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CBiStair : public CGameObject
{
public:
	CBiStair();

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	Direction upsideDirectionX;
	Direction upsideDirectionY;
	Direction downsideDirectionX;
	Direction downsideDirectionY;
};