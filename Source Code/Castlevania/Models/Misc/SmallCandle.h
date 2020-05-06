#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CSmallCandle : public CGameObject
{
public:
	CSmallCandle();

	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
};