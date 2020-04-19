#include "MoneyBag.h"
#include "../../Utilities/SafeDelete.h"

CMoneyBag::CMoneyBag()
{
	isItem = true;

	SetAnimationSet("money_bag");
}

void CMoneyBag::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	y += vy * dt;

	if (y < 338)
	{
		vy = 0;
	}
}

void CMoneyBag::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case ITEM_STATE_APPEAR:
		vy = -MONEY_BAG_MOVE_UP_SPEED;
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

void CMoneyBag::Render()
{
	if (!showingEffect)
	{
		animationSet->at(0)->Render(x, y);
	}
}

void CMoneyBag::Disappear()
{
	this->showingEffect = true;

	if (this->endingEffect)
	{
		this->endingEffect->SetVisibility(Visibility::Visible);
		this->endingEffect->SetDisplayTime(EFFECT_DISPLAY_TIME);
	}
}