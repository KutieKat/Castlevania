#pragma once
#include "Item.h"
#include "../GameObject.h"

class CEasterEgg : public CGameObject
{
public:
	CEasterEgg(CItem* hiddenItem);

	virtual void GetBoundingBox(float & left, float & top, float & right, float & bottom);
	virtual CBoundingBox GetBoundingBox();
	virtual void Render();
	virtual void ShowHiddenItem();

protected:
	CItem* hiddenItem;
};

