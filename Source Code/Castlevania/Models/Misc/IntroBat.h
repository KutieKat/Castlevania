#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CIntroBat : public CGameObject
{
public:
	CIntroBat();

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};