#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CMapPin : public CGameObject
{
public:
	CMapPin();

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};