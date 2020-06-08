#pragma once
#include "../GameObject.h"

class CEasterEgg : public CGameObject
{
public:
	CEasterEgg();

	void Render();
	void ShowHiddenItem();
	void GetBoundingBox(float &l, float &t, float &r, float &b);

	void SetMustSit(bool mustSit);
	bool MustSit();

protected:
	bool mustSit;
};

