#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CAxe : public CGameObject
{
public:
	CAxe();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Render();
};