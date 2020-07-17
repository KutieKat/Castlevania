#include "SevenHundred.h"
#include "../../Game.h"

CSevenHundred::CSevenHundred()
{
	isEffect = true;
	SetAnimationSet("seven_hundred");
	elevation = CSettingManager::GetInstance()->GetIntValue("EFFECT_DEFAULT_ELEVATION");
}

void CSevenHundred::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void CSevenHundred::Render()
{
	animationSet->at(0)->Render(x, y);
}
