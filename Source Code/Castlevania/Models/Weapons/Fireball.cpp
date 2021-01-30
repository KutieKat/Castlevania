#include "Fireball.h"
#include "../../Game.h"
#include "../Characters/Enemies/Enemy.h"

WFireball::WFireball()
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	SetAnimationSet("fireball");

	elevation = settingManager->GetIntValue("WEAPON_DEFAULT_ELEVATION");
}

void WFireball::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CSettingManager* settingManager = CSettingManager::GetInstance();

	if (y < targetY)
	{
		angle = atan2(targetY - y, targetX - x);

		x += cos(angle) * settingManager->GetFloatValue("FIREBALL_MOVE_SPEED_X") * dt;
		y += sin(angle) * settingManager->GetFloatValue("FIREBALL_MOVE_SPEED_Y") * dt;
	}
	else
	{
		x += cos(angle) * settingManager->GetFloatValue("FIREBALL_MOVE_SPEED_X") * dt;
		y += settingManager->GetFloatValue("FIREBALL_MOVE_SPEED_Y") * dt;
	}
}

void WFireball::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	left = x;
	top = y;
	right = left + FIREBALL_BBOX_WIDTH;
	bottom = top + FIREBALL_BBOX_HEIGHT;
}

void WFireball::Render()
{
	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(0)->Render(x, y);
}

void WFireball::SetTarget(float x, float y)
{
	targetX = x;
	targetY = y;
}