#include "FourHundred.h"

CFourHundred::CFourHundred()
{
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

	isEffect = true;
	SetAnimationSet("four_hundred");
	elevation = settingManager->GetIntValue("EFFECT_DEFAULT_ELEVATION");
}

void CFourHundred::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void CFourHundred::Render()
{
	animationSet->at(0)->Render(x, y);
}
