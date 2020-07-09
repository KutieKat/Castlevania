#include "SevenHundred.h"

CSevenHundred::CSevenHundred()
{
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

	isEffect = true;
	SetAnimationSet("seven_hundred");
	elevation = settingManager->GetIntValue("EFFECT_DEFAULT_ELEVATION");
}

void CSevenHundred::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void CSevenHundred::Render()
{
	animationSet->at(0)->Render(x, y);
}
