#pragma once
#include "../GameObject.h"

class CTwoThousand : public CGameObject
{
public:
	CTwoThousand();

	virtual void GetBoundingBox(float & left, float & top, float & right, float & bottom);
	virtual void Render();
};

