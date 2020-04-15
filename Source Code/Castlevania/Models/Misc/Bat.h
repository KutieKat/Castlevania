#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CBat : public CGameObject
{
private:
	float alpha;

public:
	CBat();

	void SetState(int state);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};