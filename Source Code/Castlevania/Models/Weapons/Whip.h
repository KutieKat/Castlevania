#pragma once
#include "../GameObject.h"
#include "../../Utilities/Constants.h"

class CWhip : public CGameObject
{
public:
	CWhip();

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void SetLevel(int level);
	void ResetAnimations();

protected:
	int level;
};

