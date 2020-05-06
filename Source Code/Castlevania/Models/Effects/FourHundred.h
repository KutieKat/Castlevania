#pragma once
#include "../GameObject.h"

class CFourHundred : public CGameObject
{
public:
	CFourHundred();

	virtual void GetBoundingBox(float & left, float & top, float & right, float & bottom);
	virtual void Render();
};

