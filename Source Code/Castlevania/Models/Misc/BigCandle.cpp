#include "BigCandle.h"

CBigCandle::CBigCandle()
{
	SetAnimationSet("big_candle");
}

void CBigCandle::Render()
{
	if (!showingEndingEffect)
	{
		animationSet->at(0)->Render(x, y);
	}
}

void CBigCandle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!showingEndingEffect)
	{
		CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

		left = x;
		top = y;
		right = left + settingManager->GetIntValue("BIG_CANDLE_BBOX_WIDTH");
		bottom = top + settingManager->GetIntValue("BIG_CANDLE_BBOX_HEIGHT");
	}
}