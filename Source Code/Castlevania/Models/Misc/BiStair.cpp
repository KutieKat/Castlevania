#include "BiStair.h"

CBiStair::CBiStair()
{
	SetAnimationSet("transparency");
}

void CBiStair::Render()
{
	animationSet->at(0)->Render(x, y);
}

void CBiStair::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

	left = x;
	top = y;
	right = left + settingManager->GetIntValue("BI_STAIR_BBOX_WIDTH");
	bottom = top + settingManager->GetIntValue("BI_STAIR_BBOX_HEIGHT");
}