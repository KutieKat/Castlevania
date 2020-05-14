#pragma once
#include "../GameObject.h"

class CPauseBadge : public CGameObject
{
public:
	CPauseBadge();

	void GetBoundingBox(float & left, float & top, float & right, float & bottom);
	void Render();
};

