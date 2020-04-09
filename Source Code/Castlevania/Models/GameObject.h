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

	vector<CAnimation*> animations;

public:
	CGameObject();

	void SetPosition(float x, float y) { this->x = x, this->y = y; }
	void SetSpeed(float vx, float vy) { this->vx = vx, this->vy = vy; }
	void GetPosition(float &x, float &y) { x = this->x; y = this->y; }
	void GetSpeed(float &vx, float &vy) { vx = this->vx; vy = this->vy; }
	void SetDirection(Direction direction) { this->direction = direction; }
	Direction GetDirection() { return this->direction; }

	void SetVisibility(Visibility visibility);
	Visibility GetVisibility();

	int GetState() { return this->state; }

	void RenderBoundingBox();

	LPCOLLISIONEVENT SweptAABBEx(LPGAMEOBJECT coO);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(
		vector<LPCOLLISIONEVENT> &coEvents,
		vector<LPCOLLISIONEVENT> &coEventsResult,
		float &min_tx,
		float &min_ty,
		float &nx,
		float &ny);

	void AddAnimation(string aniId);

	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) = 0;
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects = NULL);
	virtual void Render() = 0;
	virtual void SetState(int state);
	virtual void ResetAnimations();

	~CGameObject();
};

