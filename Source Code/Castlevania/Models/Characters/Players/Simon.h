#pragma once
#include "../../GameObject.h"
#include "../../../Utilities/Constants.h"
#include "../../Weapons/Whip.h"
using namespace std;

class CWhip;

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

	bool sitting;
	bool touchingGround;
	bool up;
	bool lastFrameShown;
	bool standingToWatch;
	bool onStair;
	bool atBottomStair;
	bool atTopStair;
	bool onBar;

	string movingDirection;
	string switchScenePosition;

	Direction stairDirectionX;
	Direction stairDirectionY;

	DWORD waitingTime;

	~CSimon();

protected:
	CWhip* whip;
	CGameObject* subWeapon;

	DWORD delayEndTime;
	DWORD switchSceneTime;

	void HandleGravity();
	void HandleDelay();
	void HandleAttackWithWhip(vector<LPGAMEOBJECT>* coObjects = nullptr);
	void HandleAttackWithSubWeapon(vector<LPGAMEOBJECT>* coObjects = nullptr);
	void HandleCollisionObjects(vector<LPGAMEOBJECT>* coObjects = nullptr);
	void HandleCollisionWithItems(CGameObject* item);
	void HandleSwitchScene();

	void RenderWhip();
	void RenderSubWeapon();
};