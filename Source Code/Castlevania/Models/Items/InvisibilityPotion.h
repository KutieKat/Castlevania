#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CInvisibilityPotion : public CGameObject
{
public:
	CInvisibilityPotion();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Render();
};