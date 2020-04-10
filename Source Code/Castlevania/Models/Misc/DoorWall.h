#pragma once
#include "../GameObject.h"

class CDoorWall : public CGameObject
{
public:
	virtual void GetBoundingBox(float & left, float & top, float & right, float & bottom);
	virtual void Render();
};

