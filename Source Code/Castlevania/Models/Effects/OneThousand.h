#pragma once
#include "../GameObject.h"

class COneThousand : public CGameObject
{
public:
	COneThousand();

	virtual void GetBoundingBox(float & left, float & top, float & right, float & bottom);
	virtual void Render();
};

