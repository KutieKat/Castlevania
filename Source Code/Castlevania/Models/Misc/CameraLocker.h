#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CCameraLocker : public CGameObject
{
public:
	CCameraLocker();

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

	bool isGround;
};