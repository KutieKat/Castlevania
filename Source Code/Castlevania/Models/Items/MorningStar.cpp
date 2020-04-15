#include "MorningStar.h"
#include "../../Utilities/Debug.h"

CMorningStar::CMorningStar()
{
	SetAnimationSet("morning_star");
}

void CMorningStar::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (GetTickCount() > this->disappearingTime)
	{
		this->SetVisibility(Visibility::Hidden);
	}

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

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
	}

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
	}
}

void CMorningStar::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + MORNING_STAR_BBOX_WIDTH;
	bottom = top + MORNING_STAR_BBOX_HEIGHT;
}

void CMorningStar::Render()
{
	animationSet->at(0)->Render(x, y);
}