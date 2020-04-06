#pragma once
#include "Item.h"
#include "../../Utilities/Constants.h"

class CMoneyBag : public CItem
{
public:
	CMoneyBag();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void SetState(int state);
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Render();
	void SetAmount(int amount);
	void Disappear();
	void SetEndingEffect(CEffect* effect);

	int GetAmount();

protected:
	CEffect* endingEffect;

	int amount;
	bool showingEffect;
};

