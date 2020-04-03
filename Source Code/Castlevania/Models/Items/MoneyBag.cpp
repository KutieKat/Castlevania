#include "MoneyBag.h"
#include "../../Utilities/Debug.h"

void CMoneyBag::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y -= vy * dt;
 
	if (y < 142)
	{
		vy = 0;
	}

	if (this->started && this->startEffect->Over())
	{
		this->started = false;
	}

	if (this->ended && this->endEffect->Over())
	{
		this->visibility = Visibility::Hidden;
	}
}

void CMoneyBag::SetState(int state)
{
	CItem::SetState(state);

	switch (state)
	{
	case ITEM_STATE_APPEAR:
		vy = MONEY_BAG_MOVE_UP_SPEED;
		break;
	}
}

void CMoneyBag::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!this->started && !this->ended)
	{
		left = x;
		top = y;
		right = left + MONEY_BAG_BBOX_WIDTH;
		bottom = top + MONEY_BAG_BBOX_HEIGHT;
	}
}

CBoundingBox CMoneyBag::GetBoundingBox()
{
	CBoundingBox boundingBox;

	GetBoundingBox(boundingBox.left, boundingBox.top, boundingBox.right, boundingBox.bottom);

	return boundingBox;
}

void CMoneyBag::Render()
{
	if (this->started)
	{
		this->startEffect->Start();
		this->startEffect->SetPosition(x, y);
		this->startEffect->Render();
	}
	else if (this->ended)
	{
		this->endEffect->Start();
		this->endEffect->SetPosition(x, y);
		this->endEffect->Render();
	}
	else
	{
		animations[0]->Render(x, y);
	}
}

void CMoneyBag::Appear()
{
	CItem::Appear();

	SetState(ITEM_STATE_APPEAR);
}

void CMoneyBag::SetAmount(int amount)
{
	this->amount = amount;
}

int CMoneyBag::GetAmount()
{
	return this->amount;
}
