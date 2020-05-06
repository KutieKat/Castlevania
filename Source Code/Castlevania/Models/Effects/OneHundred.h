#pragma once
#include "../GameObject.h"

class COneHundred : public CGameObject
{
public:
	COneHundred();

	virtual void GetBoundingBox(float & left, float & top, float & right, float & bottom);
	virtual void Render();
};

