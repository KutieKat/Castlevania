#include "Flash.h"

CFlash::CFlash()
{
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

	isEffect = true;
	SetAnimationSet("flash");
	elevation = settingManager->GetIntValue("EFFECT_DEFAULT_ELEVATION");
}

void CFlash::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void CFlash::Render()
{
	animationSet->at(0)->Render(x, y);
}
