#include "BiStair.h"
#include "../../Game.h"

CBiStair::CBiStair()
{
	SetAnimationSet("transparency");
}

void CBiStair::Render()
{
	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(0)->Render(x, y);
}

void CBiStair::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	left = x;
	top = y;
	right = left + BI_STAIR_BBOX_WIDTH;
	bottom = top + BI_STAIR_BBOX_HEIGHT;
}