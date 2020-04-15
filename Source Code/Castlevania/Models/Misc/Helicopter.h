#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CHelicopter : public CGameObject
{
private:
	float alpha;

public:
	CHelicopter();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};