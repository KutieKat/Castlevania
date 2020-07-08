#include "MoneyBag.h"
#include "../../Game.h"
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

	if (y < topBound)
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
		CGame::GetInstance()->GetSoundManager()->Play("showing_bonus");
		vy = -BONUS_MOVE_UP_SPEED;
		break;
	}
}

void CMoneyBag::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!showingEndingEffect)
	{
		left = x;
		top = y;
		right = left + MONEY_BAG_BBOX_WIDTH;
		bottom = top + MONEY_BAG_BBOX_HEIGHT;
	}
}

void CMoneyBag::Render()
{
	if (!showingEndingEffect)
	{
		if (CGame::GetInstance()->BoundingBoxDisplayed())
		{
			RenderBoundingBox();
		}

		animationSet->at(0)->Render(x, y);
	}
}