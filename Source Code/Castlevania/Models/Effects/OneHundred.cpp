#include "OneHundred.h"

COneHundred::COneHundred()
{
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

	isEffect = true;
	SetAnimationSet("one_hundred");
	elevation = settingManager->GetIntValue("EFFECT_DEFAULT_ELEVATION");
}

void COneHundred::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void COneHundred::Render()
{
	animationSet->at(0)->Render(x, y);
}
