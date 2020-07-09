#include "BottomStair.h"

CBottomStair::CBottomStair()
{
	SetAnimationSet("transparency");
}

void CBottomStair::Render()
{
	animationSet->at(0)->Render(x, y);
}

void CBottomStair::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

	left = x;
	top = y;
	right = left + settingManager->GetIntValue("BOTTOM_STAIR_BBOX_WIDTH");
	bottom = top + settingManager->GetIntValue("BOTTOM_STAIR_BBOX_HEIGHT");
}