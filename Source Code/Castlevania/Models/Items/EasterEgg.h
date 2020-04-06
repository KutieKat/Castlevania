#pragma once
#include "Item.h"
#include "../GameObject.h"

class CEasterEgg : public CGameObject
{
public:
	void Render();
	void SetHiddenItem(CItem* item);
	void ShowHiddenItem();
	void GetBoundingBox(float &l, float &t, float &r, float &b);

protected:
	CItem* hiddenItem;
};

