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
	int jumpable;

	bool partiallyInvisible;
	bool fullyInvisible;
	bool sitting;
	bool touchingGround;
	bool up;
	bool down;
	bool lastFrameShown;
	bool standingToWatch;
	bool onStair;
	bool atTopStair;
	bool atBottomStair;
	bool atBiStair;
	bool onBar;
	bool deflecting;
	bool switchSceneEnabled;

	bool AbleToThrowSubWeapon();

	string movingDirection;
	string switchScenePosition;

	Direction stairDirectionX;
	Direction stairDirectionY;

	Direction upsideDirectionX;
	Direction upsideDirectionY;
	Direction downsideDirectionX;
	Direction downsideDirectionY;

	DWORD waitingTime;
	DWORD invisibleTimeout;
	
	~CSimon();

protected:
	CWhip* whip;
	CGameObject* subWeapon;

	DWORD delayEndTime;
	DWORD switchSceneTime;

	int sittingCounter;

	void HandleGravity();
	void HandleDelay();
	void HandleAttackWithWhip(vector<LPGAMEOBJECT>* coObjects = nullptr);
	void HandleAttackWithSubWeapon(vector<LPGAMEOBJECT>* coObjects = nullptr);
	void HandleCollisionObjects(vector<LPGAMEOBJECT>* coObjects = nullptr);
	void HandleCollisionWithItems(CGameObject* item);
	void HandleCollisionWithEnemies(CGameObject* item);
	void HandleSwitchScene();
	void HandleInvisibility();

	void RenderWhip();
};