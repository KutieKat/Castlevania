#include "NextScene.h"

CNextScene::CNextScene()
{
	playerMustBeOnStair = false;
	SetAnimationSet("transparency");
}

void CNextScene::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + NEXT_SCENE_BBOX_WIDTH;
	bottom = top + NEXT_SCENE_BBOX_HEIGHT;
}

void CNextScene::Render()
{
	animationSet->at(0)->Render(x, y);
}