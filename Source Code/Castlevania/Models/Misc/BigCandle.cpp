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

//void CBigCandle::Disappear()
//{
//	//if (endingEffect)
//	//{
//	//	showingEffect = true;
//
//	//	endingEffect->SetVisibility(Visibility::Visible);
//	//	endingEffect->SetPosition(x, y);
//	//	endingEffect->SetDisplayTime(EFFECT_DISPLAY_TIME);
//	//}
//}
