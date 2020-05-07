#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CBreakableBrick : public CGameObject
{
public:
	CBreakableBrick();

	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);

	bool isGround;
};