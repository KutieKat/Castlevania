#include "BigCandle.h"
#include "../../Utilities/SafeDelete.h"
#include "../../Utilities/Debug.h"

CBigCandle::CBigCandle()
{
	this->showingEffect = false;

	SetAnimationSet("big_candle");
}

void CBigCandle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->endingEffect && this->endingEffect->Over())
	{
		if (this->hiddenItem)
		{
			this->hiddenItem->SetVisibility(Visibility::Visible);
			this->hiddenItem->SetDisplayTime(ITEM_DISPLAY_TIME);
		}

		this->visibility = Visibility::Hidden;
	}
}

void CBigCandle::Render()
{
	if (!showingEffect)
	{
		animationSet->at(0)->Render(x, y);
	}
}

void CBigCandle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!showingEffect)
	{
		left = x;
		top = y;
		right = left + BIG_CANDLE_BBOX_WIDTH;
		bottom = top + BIG_CANDLE_BBOX_HEIGHT;
	}
}

void CBigCandle::Disappear()
{
	this->showingEffect = true;

	if (this->endingEffect)
	{
		this->endingEffect->SetPosition(x, y);
		this->endingEffect->SetStartTime(GetTickCount());
	}
}

void CBigCandle::SetEndingEffect(CEffect* effect)
{
	this->endingEffect = effect;
}

void CBigCandle::SetHiddenItem(CItem* item)
{
	this->hiddenItem = item;
	this->hiddenItem->SetVisibility(Visibility::Hidden);
}

CBigCandle::~CBigCandle()
{
	SAFE_DELETE(this->endingEffect);
	SAFE_DELETE(this->hiddenItem);
}
