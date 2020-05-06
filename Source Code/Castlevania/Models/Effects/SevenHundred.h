#pragma once
#include "../GameObject.h"

class CSevenHundred : public CGameObject
{
public:
	CSevenHundred();

	virtual void GetBoundingBox(float & left, float & top, float & right, float & bottom);
	virtual void Render();
};

