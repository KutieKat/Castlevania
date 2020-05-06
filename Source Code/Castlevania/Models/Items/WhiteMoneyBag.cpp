#include "WhiteMoneyBag.h"

CWhiteMoneyBag::CWhiteMoneyBag()
{
	isItem = true;
	SetAnimationSet("white_money_bag");
}

void CWhiteMoneyBag::Render()
{
	if (!showingEffect)
	{
		animationSet->at(0)->Render(x, y);
	}
}

void CWhiteMoneyBag::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
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

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;
	}

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
	}
}

void CWhiteMoneyBag::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!showingEffect)
	{
		left = x;
		top = y;
		right = left + WHITE_MONEY_BAG_BBOX_WIDTH;
		bottom = top + WHITE_MONEY_BAG_BBOX_HEIGHT;
	}
}