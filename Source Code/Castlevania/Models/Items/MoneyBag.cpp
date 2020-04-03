#include "MoneyBag.h"
#include "../../Utilities/Debug.h"

void CMoneyBag::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	y -= vy * dt;
 
	if (y < 142)
	{
		vy = 0;
	}

	if (this->started && this->startingEffect->Over())
	{
		this->started = false;
		this->startingEffect->Reset();
	}

	if (this->ended && this->endingEffect->Over())
	{
		this->visibility = Visibility::Hidden;
		this->endingEffect->Reset();
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
		this->startingEffect->Start();
		this->startingEffect->SetPosition(x, y);
		this->startingEffect->Render();
	}
	else if (this->ended)
	{
		this->endingEffect->Start();
		this->endingEffect->SetPosition(x, y);
		this->endingEffect->Render();
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
