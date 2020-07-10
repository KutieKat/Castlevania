#include "MagicCrystal.h"
#include "../Misc/SmallCandle.h"
#include "../Misc/BottomStair.h"
#include "../Weapons/Bone.h"
#include "../Weapons/Fireball.h"
#include "../Weapons/HolyWaterBottle.h"
#include "../Weapons/WAxe.h"
#include "../Weapons/WBoomerang.h"
#include "../Weapons/WDagger.h"
#include "../Weapons/WStopwatch.h"
#include "../Characters/Enemies/Enemy.h"
#include "../../Game.h"

CMagicCrystal::CMagicCrystal()
{
	isItem = true;
	SetAnimationSet("magic_crystal");
}

void CMagicCrystal::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CSettingManager* settingManager = CSettingManager::GetInstance();

	vy += settingManager->GetFloatValue("ITEM_GRAVITY") * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBottomStair*>(e->obj))
			{
				if (e->ny != 0) y += dy;
			}
			else if (dynamic_cast<CSmallCandle*>(e->obj))
			{
				if (e->ny != 0) y += dy;
			}
			else if (e->obj->isItem)
			{
				y -= min_ty * dy + ny * 0.4f;

				if (e->ny != 0) y += dy;
			}
			else if (dynamic_cast<WBone*>(e->obj) || dynamic_cast<WFireball*>(e->obj) || dynamic_cast<CHolyWaterBottle*>(e->obj) || dynamic_cast<WAxe*>(e->obj) || dynamic_cast<WBoomerang*>(e->obj) || dynamic_cast<WDagger*>(e->obj) || dynamic_cast<WStopwatch*>(e->obj))
			{
				y -= min_ty * dy + ny * 0.4f;

				if (e->ny != 0) y += dy;
			}
			else if (dynamic_cast<CEnemy*>(e->obj))
			{
				y -= min_ty * dy + ny * 0.4f;

				if (e->ny < 0) y += dy;
			}
			else
			{
				if (e->ny != 0) vy = 0;
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
	}
}

void CMagicCrystal::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	left = x;
	top = y;
	right = left + settingManager->GetIntValue("MAGIC_CRYSTAL_BBOX_WIDTH");
	bottom = top + settingManager->GetIntValue("MAGIC_CRYSTAL_BBOX_HEIGHT");
}

void CMagicCrystal::Render()
{
	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(0)->Render(x, y);
}