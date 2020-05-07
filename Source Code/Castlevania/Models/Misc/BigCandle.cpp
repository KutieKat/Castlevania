#include "BigCandle.h"

CBigCandle::CBigCandle()
{
	SetAnimationSet("big_candle");
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