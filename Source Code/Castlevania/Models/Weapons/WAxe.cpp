#include "WAxe.h"
#include "../../Game.h"
#include "../Characters/Enemies/Enemy.h"
#include "../Misc/Brick.h"
#include "../Misc/Ground.h"
#include "../Misc/BottomStair.h"

WAxe::WAxe()
{
	SetAnimationSet("axe");
	vy = -AXE_MOVE_SPEED;
}

void WAxe::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vx = directionX == Direction::Right ? AXE_MOVE_SPEED : -AXE_MOVE_SPEED;

	vy += AXE_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (coObjects) CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (x > CGame::GetInstance()->GetCamera()->GetRight())
		{
			removable = true;
		}
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CEnemy*>(e->obj))
			{
				auto enemy = dynamic_cast<CEnemy*>(e->obj);

				enemy->TakeDamage();
				removable = true;
			}
			else if (dynamic_cast<CBrick*>(e->obj) || dynamic_cast<CGround*>(e->obj))
			{
				removable = true;
			}
			else if (dynamic_cast<CBottomStair*>(e->obj))
			{
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
			else
			{
				if (e->nx != 0) x += dx;
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
	}
}

void WAxe::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + AXE_BBOX_WIDTH;
	bottom = top + AXE_BBOX_HEIGHT;
}

void WAxe::Render()
{
	animationSet->at(AXE_ANI_MOVE)->Render(x, y);
}
