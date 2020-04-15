#pragma once
#include <Windows.h>
#include <d3dx9.h>
#include <vector>
#include <string>

#include "../Animations/Animation.h"
#include "../Animations/AnimationManager.h"
#include "../Effects/Effect.h"
#include "../Sprites/SpriteManager.h"
#include "../Utilities/Constants.h"
using namespace std;

class CGameObject
{
public:
	float x;
	float y;

	float dx;	// dx = vx*dt
	float dy;	// dy = vy*dt

	float vx;
	float vy;

	Direction direction;
	Visibility visibility;

	int state;

	DWORD dt;

	CAnimationSet* animationSet;

public:
	CGameObject();

	void SetPosition(float x, float y);
	void SetSpeed(float vx, float vy);
	void GetPosition(float &x, float &y);
	void GetSpeed(float &vx, float &vy);
	void SetDirection(Direction direction);
	Direction GetDirection();

	void SetVisibility(Visibility visibility);
	Visibility GetVisibility();

	int GetState();

	void RenderBoundingBox();

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
		float &rdy);

	void SetAnimationSet(string animationSetId);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = nullptr);
	virtual void Render() = 0;
	virtual void SetState(int state);
	virtual void ResetAnimations();

	virtual void Disappear() {};

	~CGameObject();
};

