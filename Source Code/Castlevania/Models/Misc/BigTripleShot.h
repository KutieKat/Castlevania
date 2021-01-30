#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CBigTripleShot : public CGameObject
{
public:
	CBigTripleShot(bool blinking = false);

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);

protected:
	bool blinking;
};