#pragma once
#include "../GameObject.h"
#include "../Items/Item.h"
#include "../../Utilities/Constants.h"

class CBigCandle : public CGameObject
{
public:
	CBigCandle();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void GetBoundingBox(float &l, float &t, float &r, float &b);
	void Disappear();
	void SetEndingEffect(CEffect* effect);
	void SetHiddenItem(CItem* item);

protected:
	CEffect* endingEffect;
	CItem* hiddenItem;

	bool showingEffect;
};