#include "TopStair.h"
#include "../../Game.h"

CTopStair::CTopStair()
{
	SetAnimationSet("transparency");
}

void CTopStair::Render()
{
	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(0)->Render(x, y);
}

void CTopStair::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	left = x;
	top = y;
	right = left + settingManager->GetIntValue("TOP_STAIR_BBOX_WIDTH");
	bottom = top + settingManager->GetIntValue("TOP_STAIR_BBOX_HEIGHT");
}