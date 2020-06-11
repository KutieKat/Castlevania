#include "TripleShot.h"
#include "../Misc/BottomStair.h"
#include "../Weapons/HolyWaterBottle.h"

CTripleShot::CTripleShot()
{
	isItem = true;
	SetAnimationSet("triple_shot");
}

void CTripleShot::Render()
{
	animationSet->at(0)->Render(x, y);
}

void CTripleShot::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vy += ITEM_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
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

			if (dynamic_cast<CBottomStair*>(e->obj))
			{
				if (e->ny != 0) y += dy;
			}
			else if (e->obj->isItem)
			{
				if (e->ny != 0) y += dy;
			}
			else if (dynamic_cast<CHolyWaterBottle*>(e->obj))
			{
				if (e->ny != 0) y += dy;
			}
			else
			{
				if (e->ny != 0) vy = 0;
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
	}
}

void CTripleShot::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + TRIPLE_SHOT_BBOX_WIDTH;
	bottom = top + TRIPLE_SHOT_BBOX_HEIGHT;
}