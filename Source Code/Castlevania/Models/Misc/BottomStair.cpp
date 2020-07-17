#include "BottomStair.h"
#include "../../Game.h"

CBottomStair::CBottomStair()
{
	SetAnimationSet("transparency");
}

void CBottomStair::Render()
{
	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(0)->Render(x, y);
}

void CBottomStair::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	left = x;
	top = y;
	right = left + BOTTOM_STAIR_BBOX_WIDTH;
	bottom = top + BOTTOM_STAIR_BBOX_HEIGHT;
}