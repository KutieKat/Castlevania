#include "OneHundred.h"
#include "../../Game.h"

COneHundred::COneHundred()
{
	isEffect = true;
	SetAnimationSet("one_hundred");
	elevation = CSettingManager::GetInstance()->GetIntValue("EFFECT_DEFAULT_ELEVATION");
}

void COneHundred::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void COneHundred::Render()
{
	animationSet->at(0)->Render(x, y);
}
