#include "PreviousScene.h"

CPreviousScene::CPreviousScene()
{
	playerMustBeOnStair = false;
	SetAnimationSet("transparency");
}

void CPreviousScene::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

	left = x;
	top = y;
	right = left + settingManager->GetIntValue("PREVIOUS_SCENE_BBOX_WIDTH");
	bottom = top + settingManager->GetIntValue("PREVIOUS_SCENE_BBOX_HEIGHT");
}

void CPreviousScene::Render()
{
	animationSet->at(0)->Render(x, y);
}