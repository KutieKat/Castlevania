#include "WStopwatch.h"
#include "../../Game.h"
#include "../Characters/Enemies/Enemy.h"
#include "../Misc/Brick.h"

WStopwatch::WStopwatch()
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	SetAnimationSet("stopwatch");

	elevation = settingManager->GetIntValue("WEAPON_DEFAULT_ELEVATION");
	vy = -settingManager->GetFloatValue("STOPWATCH_MOVE_SPEED");
}

void WStopwatch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CSettingManager* settingManager = CSettingManager::GetInstance();

	vx = directionX == Direction::Right ? settingManager->GetFloatValue("STOPWATCH_MOVE_SPEED") : -settingManager->GetFloatValue("STOPWATCH_MOVE_SPEED");

	vy += settingManager->GetFloatValue("STOPWATCH_GRAVITY") * dt;

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

			if (dynamic_cast<CBrick*>(e->obj))
			{
				CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->SoftPause(settingManager->GetIntValue("DEFAULT_SOFT_PAUSE_TIME"));
				CGame::GetInstance()->GetPlayerData()->DecreaseThrownSubWeapons();

				removable = true;
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

void WStopwatch::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	left = x;
	top = y;
	right = left + STOPWATCH_BBOX_WIDTH;
	bottom = top + STOPWATCH_BBOX_HEIGHT;
}

void WStopwatch::Render()
{
	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(0)->Render(x, y);
}
