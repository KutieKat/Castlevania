#include "MoneyBag.h"
#include "../../Utilities/SafeDelete.h"

CMoneyBag::CMoneyBag()
{
	this->showingEffect = false;

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

	if (GetTickCount() > this->disappearingTime)
	{
		this->SetVisibility(Visibility::Hidden);
	}

	if (this->endingEffect->Over())
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
		vy = -MONEY_BAG_MOVE_UP_SPEED;
		break;
	}
}

void CMoneyBag::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!showingEffect)
	{
		left = x;
		top = y;
		right = left + MONEY_BAG_BBOX_WIDTH;
		bottom = top + MONEY_BAG_BBOX_HEIGHT;
	}
}

void CMoneyBag::Render()
{
	if (!showingEffect)
	{
		animationSet->at(0)->Render(x, y);
	}
}

void CMoneyBag::SetScore(int score)
{
	this->score = score;
}

void CMoneyBag::SetEndingEffect(CEffect* effect)
{
	this->endingEffect = effect;
}

int CMoneyBag::GetScore()
{
	return this->score;
}

void CMoneyBag::Disappear()
{
	this->showingEffect = true;
	this->endingEffect->SetPosition(x, y);
	this->endingEffect->SetStartTime(GetTickCount());
}

CMoneyBag::~CMoneyBag()
{
	SAFE_DELETE(this->endingEffect);
}