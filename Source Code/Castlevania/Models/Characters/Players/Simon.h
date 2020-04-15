#pragma once
#include "../../GameObject.h"
#include "../../../Utilities/Constants.h"
#include "../../Weapons/Weapon.h"
#include "../../Items/Item.h"
using namespace std;

class CSimon : public CGameObject
{
public:
	CSimon();

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = nullptr);
	void Render();
	void SetState(int state);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void ResetAnimations();

	int GetAnimationToRender();

	bool Sitting();
	bool TouchingGround();
	bool Up();

	void SetUp(bool up);

	~CSimon();

protected:
	CWeapon* whip;
	CWeapon* subWeapon;

	bool sitting;
	bool touchingGround;
	bool up;

	DWORD delayEndTime;
	DWORD switchSceneTime;

	void InitSubWeapon();
	void UpdateWhip();
};