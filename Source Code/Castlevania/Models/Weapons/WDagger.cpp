#include "WDagger.h"
#include "../../Game.h"
#include "../Characters/Enemies/Enemy.h"
#include "../Misc/BigCandle.h"
#include "../Misc/SmallCandle.h"

WDagger::WDagger()
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	SetAnimationSet("dagger");

	elevation = settingManager->GetIntValue("WEAPON_DEFAULT_ELEVATION");
}

void WDagger::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CSettingManager* settingManager = CSettingManager::GetInstance();

	vx = directionX == Direction::Right ? settingManager->GetFloatValue("DAGGER_MOVE_SPEED") : -settingManager->GetFloatValue("DAGGER_MOVE_SPEED");

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (coObjects) CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (x > CGame::GetInstance()->GetCamera()->GetRight() || x < CGame::GetInstance()->GetCamera()->GetLeft())
		{
			CGame::GetInstance()->GetPlayerData()->DecreaseThrownSubWeapons();

			removable = true;
		}
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

			if (dynamic_cast<CEnemy*>(e->obj))
			{
				auto enemy = dynamic_cast<CEnemy*>(e->obj);

				enemy->TakeDamage(settingManager->GetIntValue("DAGGER_DAMAGES"));

				if (e->nx != 0) x += dx;

				CGame::GetInstance()->GetSoundManager()->Play("taking_damage");
			}
			else if (dynamic_cast<CBigCandle*>(e->obj))
			{
				auto candle = dynamic_cast<CBigCandle*>(e->obj);

				candle->Disappear();

				if (e->nx != 0) x += dx;

				CGame::GetInstance()->GetSoundManager()->Play("taking_damage");
			}
			else if (dynamic_cast<CSmallCandle*>(e->obj))
			{
				auto candle = dynamic_cast<CSmallCandle*>(e->obj);

				candle->Disappear();

				if (e->nx != 0) x += dx;

				CGame::GetInstance()->GetSoundManager()->Play("taking_damage");
			}
			else
			{
				if (e->nx != 0) x += dx;
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
	}
}

void WDagger::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	left = x;
	top = y;
	right = left + settingManager->GetIntValue("DAGGER_BBOX_WIDTH");
	bottom = top + settingManager->GetIntValue("DAGGER_BBOX_HEIGHT");
}

void WDagger::Render()
{
	int ani = directionX == Direction::Right ? DAGGER_ANI_RIGHT : DAGGER_ANI_LEFT;

	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(ani)->Render(x, y);
}
