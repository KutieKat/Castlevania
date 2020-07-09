#include "TopStair.h"

CTopStair::CTopStair()
{
	SetAnimationSet("transparency");
}

void CTopStair::Render()
{
	animationSet->at(0)->Render(x, y);
}

void CTopStair::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

	left = x;
	top = y;
	right = left + settingManager->GetIntValue("TOP_STAIR_BBOX_WIDTH");
	bottom = top + settingManager->GetIntValue("TOP_STAIR_BBOX_HEIGHT");
}