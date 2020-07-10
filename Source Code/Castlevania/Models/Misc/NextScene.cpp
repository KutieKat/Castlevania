#include "NextScene.h"
#include "../../Game.h"

CNextScene::CNextScene()
{
	playerMustBeOnStair = false;
	SetAnimationSet("transparency");
}

void CNextScene::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	left = x;
	top = y;
	right = left + settingManager->GetIntValue("NEXT_SCENE_BBOX_WIDTH");
	bottom = top + settingManager->GetIntValue("NEXT_SCENE_BBOX_HEIGHT");
}

void CNextScene::Render()
{
	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(0)->Render(x, y);
}