#pragma once
#include "../GameObject.h"

class CFlash : public CGameObject
{
public:
	CFlash();

	virtual void GetBoundingBox(float & left, float & top, float & right, float & bottom);
	virtual void Render();
};

