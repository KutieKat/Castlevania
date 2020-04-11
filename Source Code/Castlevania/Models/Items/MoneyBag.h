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
	void SetScore(int score);
	void Disappear();
	void SetEndingEffect(CEffect* effect);

	int GetScore();

protected:
	CEffect* endingEffect;

	int score;
	bool showingEffect;
};

