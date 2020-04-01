#pragma once
#include "../../GameObject.h"
#include "../../../Utilities/Constants.h"
#include "../../Weapons/Whip.h"

class CSimon : public CGameObject
{
public:
	CSimon();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	void Render();
	void SetState(int state);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void ResetAnimations();

	int GetAnimationToRender();
	CBoundingBox GetBoundingBox();

	bool Sitting();
	bool Standing();
	bool AnimationFinished();

protected:
	CWhip* whip;

	bool sitting;
	bool standing;
	bool animationFinished;
};