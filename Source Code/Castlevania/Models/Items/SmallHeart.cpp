#include "SmallHeart.h"
#include "../../Game.h"

CSmallHeart::CSmallHeart()
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	vy = settingManager->GetFloatValue("SMALL_HEART_MOVE_SPEED");
	isItem = true;
	SetAnimationSet("small_heart");
}

void CSmallHeart::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	y += dy;
	x += sin(y * 10 * PI / 180);
}

void CSmallHeart::Render()
{
	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(0)->Render(x, y);
}

void CSmallHeart::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	left = x;
	top = y;
	right = left + SMALL_HEART_BBOX_WIDTH;
	bottom = top + SMALL_HEART_BBOX_HEIGHT;
}