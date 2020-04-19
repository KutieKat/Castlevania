#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CDagger : public CGameObject
{
public:
	CDagger();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Render();
};