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
		CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

		left = x;
		top = y + 10;
		right = left + settingManager->GetIntValue("SMALL_CANDLE_BBOX_WIDTH");
		bottom = top + settingManager->GetIntValue("SMALL_CANDLE_BBOX_HEIGHT");
	}
}
