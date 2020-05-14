#pragma once
#include "../GameObject.h"
#include "../Characters/Players/Simon.h"
#include "../../Utilities/Constants.h"

class CSimon;

class CWhip : public CGameObject
{
public:
	CWhip(CSimon* simon);

	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void ResetAnimations();

	bool HaveCollision(CGameObject* object);

protected:
	CSimon* simon;

	int GetAnimationToRender();
};

