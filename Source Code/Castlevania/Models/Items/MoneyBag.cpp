#include "MoneyBag.h"

void CMoneyBag::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y -= vy * dt;

	if (y < 142)
	{
		SetState(MONEY_BAG_STATE_IDLE);
	}
}

void CMoneyBag::SetState(int state)
{
	CItem::SetState(state);

	switch (state)
	{
	case MONEY_BAG_STATE_MOVE_UP:
		vy = MONEY_BAG_MOVE_UP_SPEED;
		break;

	case MONEY_BAG_STATE_IDLE:
		vy = 0;
		break;
	}
}

void CMoneyBag::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + MONEY_BAG_BBOX_WIDTH;
	bottom = top + MONEY_BAG_BBOX_HEIGHT;
}

CBoundingBox CMoneyBag::GetBoundingBox()
{
	CBoundingBox boundingBox;

	GetBoundingBox(boundingBox.left, boundingBox.top, boundingBox.right, boundingBox.bottom);

	return boundingBox;
}

void CMoneyBag::Render()
{
	animations[0]->Render(x, y);
}
