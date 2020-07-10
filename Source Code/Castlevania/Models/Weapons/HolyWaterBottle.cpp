#include "HolyWaterBottle.h"
#include "../../Game.h"
#include "../Characters/Enemies/Enemy.h"
#include "../Misc/Brick.h"

CHolyWaterBottle::CHolyWaterBottle()
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	SetAnimationSet("holy_water_bottle");

	elevation = settingManager->GetIntValue("WEAPON_DEFAULT_ELEVATION");
	vy = -settingManager->GetFloatValue("HOLY_WATER_BOTTLE_MOVE_SPEED");

	SetState(settingManager->GetIntValue("HOLY_WATER_INITIAL_STATE"));
}

void CHolyWaterBottle::SetState(int state)
{
	CGameObject::SetState(state);
	CSettingManager* settingManager = CSettingManager::GetInstance();

	switch (state)
	{
	case HOLY_WATER_BOTTLE_STATE_MOVE:
		break;

	case HOLY_WATER_BOTTLE_STATE_BURN:
		vx = vy = 0;
		SetDisplayTime(settingManager->GetIntValue("HOLY_WATER_BOTTLE_BURNING_TIME"));

		CGame::GetInstance()->GetSoundManager()->Play("holy_water_breaking");
		CGame::GetInstance()->GetPlayerData()->DecreaseThrownSubWeapons();
		break;
	}
}

void CHolyWaterBottle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CSettingManager* settingManager = CSettingManager::GetInstance();

	if (state != HOLY_WATER_BOTTLE_STATE_BURN)
	{
		vx = directionX == Direction::Right ? settingManager->GetFloatValue("HOLY_WATER_BOTTLE_MOVE_SPEED") : -settingManager->GetFloatValue("HOLY_WATER_BOTTLE_MOVE_SPEED");
		vy += settingManager->GetFloatValue("HOLY_WATER_BOTTLE_GRAVITY") * dt;
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (coObjects) CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (y < CGame::GetInstance()->GetCamera()->GetTop() || y > CGame::GetInstance()->GetCamera()->GetBottom() || x > CGame::GetInstance()->GetCamera()->GetRight() || x < CGame::GetInstance()->GetCamera()->GetLeft())
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

			if (dynamic_cast<CBrick*>(e->obj))
			{
				if (e->nx != 0) vx = 0;
				if (e->ny != 0) vy = 0;

				SetState(HOLY_WATER_BOTTLE_STATE_BURN);
			}
			else if (dynamic_cast<CEnemy*>(e->obj))
			{
				auto enemy = dynamic_cast<CEnemy*>(e->obj);

				if (state == HOLY_WATER_BOTTLE_STATE_BURN)
				{
					enemy->TakeDamage(settingManager->GetIntValue("HOLY_WATER_BOTTLE_DAMAGES"));
				}
				else
				{
					if (e->nx != 0) x += dx;
				}
			}
			else
			{
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
	}
}

void CHolyWaterBottle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	left = x;
	top = y;
	right = left + settingManager->GetIntValue("HOLY_WATER_BOTTLE_BBOX_WIDTH");
	bottom = top + settingManager->GetIntValue("HOLY_WATER_BOTTLE_BBOX_HEIGHT");
}

void CHolyWaterBottle::Render()
{
	int ani;

	switch (state)
	{
	case HOLY_WATER_BOTTLE_STATE_MOVE:
		ani = directionX == Direction::Right ? HOLY_WATER_BOTTLE_ANI_RIGHT : HOLY_WATER_BOTTLE_ANI_LEFT;
		break;

	case HOLY_WATER_BOTTLE_STATE_BURN:
		ani = HOLY_WATER_BOTTLE_ANI_FLAME;
		break;
	}

	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}
	
	animationSet->at(ani)->Render(x, y);
}
