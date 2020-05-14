#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include <string>

#include "../Animations/Animation.h"
#include "../Animations/AnimationManager.h"
#include "../Sprites/SpriteManager.h"
#include "../Utilities/Constants.h"
using namespace std;

class CGameObject
{
public:
	string id;

	float x;
	float y;
	float dx;
	float dy;
	float vx;
	float vy;

	Direction directionX;
	Direction directionY;

	Visibility visibility;

	int state;
	int counter;

	int leftBound;
	int topBound;
	int rightBound;
	int bottomBound;

	DWORD dt;
	DWORD disappearingTime;

	CAnimationSet* animationSet;

	CGameObject* hiddenItem;
	CGameObject* endingEffect;

	bool showingEffect;
	bool isEffect;
	bool isItem;
	bool removable;

	int remainingDisplayTime;

public:
	CGameObject();

	void SetPosition(float x, float y);
	void SetSpeed(float vx, float vy);
	void GetPosition(float &x, float &y);
	void GetSpeed(float &vx, float &vy);

	void SetDirectionX(Direction direction);
	Direction GetDirectionX();

	void SetDirectionY(Direction direction) { directionY = direction; }
	Direction GetDirectionY() { return directionY; };

	void SetId(string id);
	string GetId();

	void SetVisibility(Visibility visibility);
	Visibility GetVisibility();

	int GetState();

	void RenderBoundingBox();

	void SetLeftBound(int left);
	void SetTopBound(int top);
	void SetRightBound(int right);
	void SetBottomBound(int bottom);

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents,
		vector<LPCOLLISIONEVENT> &coEventsResult,
		float &min_tx,
		float &min_ty,
		float &nx,
		float &ny,
		float &rdx,
		float &rdy
	);

	void SetAnimationSet(string animationSetId);
	void SetDisplayTime(DWORD time);
	void SetHiddenItem(CGameObject* item);

	CGameObject* GetHiddenItem();

	void SetEndingEffect(CGameObject* effect);

	bool Over();

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = nullptr);
	virtual void Render() = 0;
	virtual void SetState(int state);
	virtual void ResetAnimations();
	virtual void Disappear();
	virtual void ShowHiddenItem();

	~CGameObject();
};

