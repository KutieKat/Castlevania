#pragma once
#include "Item.h"
#include "../../Utilities/Constants.h"

class CMoneyBag : public CItem
{
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void SetState(int state);
	virtual void GetBoundingBox(float & left, float & top, float & right, float & bottom);
	virtual CBoundingBox GetBoundingBox();
	virtual void Render();
};

