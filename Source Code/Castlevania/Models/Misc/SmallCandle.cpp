#include "SmallCandle.h"
#include "../../Game.h"

CSmallCandle::CSmallCandle()
{
	SetAnimationSet("small_candle");
}

void CSmallCandle::Render()
{
	if (!showingEndingEffect)
	{
		animationSet->at(0)->Render(x, y);
	}
}

void CSmallCandle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!showingEndingEffect)
	{
		left = x;
		top = y + 10;
		right = left + SMALL_CANDLE_BBOX_WIDTH;
		bottom = top + SMALL_CANDLE_BBOX_HEIGHT;
	}
}
