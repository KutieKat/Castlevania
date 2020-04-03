#pragma once
#include "Item.h"
#include "../../Utilities/Constants.h"

class CMoneyBag : public CItem
{
public:
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void SetState(int state);
	virtual void Render();

	virtual void Appear();

	virtual void GetBoundingBox(float & left, float & top, float & right, float & bottom);
	virtual CBoundingBox GetBoundingBox();

	virtual void SetAmount(int amount);
	virtual int GetAmount();

protected:
	int amount;
};

