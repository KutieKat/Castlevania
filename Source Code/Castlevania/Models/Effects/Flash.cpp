#include "Flash.h"
#include "../../Game.h"

CFlash::CFlash()
{
	isEffect = true;
	SetAnimationSet("flash");
	elevation = CSettingManager::GetInstance()->GetIntValue("EFFECT_DEFAULT_ELEVATION");
}

void CFlash::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void CFlash::Render()
{
	animationSet->at(0)->Render(x, y);
}
