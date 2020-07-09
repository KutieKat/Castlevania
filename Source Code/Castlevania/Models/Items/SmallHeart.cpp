#include "SmallHeart.h"

CSmallHeart::CSmallHeart()
{
	vy = CGame::GetInstance()->GetSettingManager()->GetFloatValue("SMALL_HEART_MOVE_SPEED");
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
	animationSet->at(0)->Render(x, y);
}

void CSmallHeart::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

	left = x;
	top = y;
	right = left + settingManager->GetIntValue("SMALL_HEART_BBOX_WIDTH");
	bottom = top + settingManager->GetIntValue("SMALL_HEART_BBOX_HEIGHT");
}