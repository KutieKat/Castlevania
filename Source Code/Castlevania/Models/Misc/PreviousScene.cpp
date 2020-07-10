#include "PreviousScene.h"
#include "../../Game.h"

CPreviousScene::CPreviousScene()
{
	playerMustBeOnStair = false;
	SetAnimationSet("transparency");
}

void CPreviousScene::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	left = x;
	top = y;
	right = left + settingManager->GetIntValue("PREVIOUS_SCENE_BBOX_WIDTH");
	bottom = top + settingManager->GetIntValue("PREVIOUS_SCENE_BBOX_HEIGHT");
}

void CPreviousScene::Render()
{
	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(0)->Render(x, y);
}