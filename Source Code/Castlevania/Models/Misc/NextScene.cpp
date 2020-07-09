#include "NextScene.h"

CNextScene::CNextScene()
{
	playerMustBeOnStair = false;
	SetAnimationSet("transparency");
}

void CNextScene::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

	left = x;
	top = y;
	right = left + settingManager->GetIntValue("NEXT_SCENE_BBOX_WIDTH");
	bottom = top + settingManager->GetIntValue("NEXT_SCENE_BBOX_HEIGHT");
}

void CNextScene::Render()
{
	animationSet->at(0)->Render(x, y);
}