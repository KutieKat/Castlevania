#include "Ghost.h"
#include <cstdlib>
#include <time.h>

#include "../../../Game.h"
#include "../../Misc/BottomStair.h"
#include "../../Weapons/HolyWaterBottle.h"

CGhost::CGhost(CSimon* simon)
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	this->simon = simon;
	this->attacks = settingManager->GetIntValue("GHOST_ATTACKS");

	SetAnimationSet("ghost");
	SetState(settingManager->GetIntValue("GHOST_INITIAL_STATE"));

	srand(time(NULL));
}

void CGhost::SetState(int state)
{
	CGameObject::SetState(state);
	CSettingManager* settingManager = CSettingManager::GetInstance();

	switch (state)
	{
	case GHOST_STATE_MOVE:
		vx = settingManager->GetFloatValue("GHOST_MOVE_SPEED_X");
		vy = settingManager->GetFloatValue("GHOST_MOVE_SPEED_Y");
		break;

	case GHOST_STATE_DELAY:
		vx = vy = 0;
		delayTimeout = GetTickCount() + settingManager->GetIntValue("ENEMY_DELAY_TIME");
		break;
	}
}

void CGhost::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CSettingManager* settingManager = CSettingManager::GetInstance();

	bool softPaused = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->SoftPaused();

	if (softPaused) return;

	if (state == GHOST_STATE_DELAY && delayTimeout != -1 && GetTickCount() > delayTimeout)
	{
		if (GetTickCount() > delayTimeout + settingManager->GetIntValue("ENEMY_DELAY_PLUS_TIME"))
		{
			delayTimeout = -1;
		}

		SetState(GHOST_STATE_MOVE);
	}

	if (directionX == Direction::Right)
	{
		x += vx * dt;

		if (x > simon->x + RandomizeDistance())
		{
			directionX = Direction::Left;
		}
	}
	else
	{
		x -= vx * dt;

		if (x < simon->x - RandomizeDistance())
		{
			directionX = Direction::Right;
		}
	}

	if (y < simon->y + 20)
	{
		y += ceil(vy * dt);
	}
	else
	{
		y -= ceil(vy * dt);
	}
}

void CGhost::Render()
{
	int ani;

	switch (state)
	{
	case GHOST_STATE_MOVE:
		ani = directionX == Direction::Right ? GHOST_ANI_MOVE_RIGHT : GHOST_ANI_MOVE_LEFT;
		break;

	case GHOST_STATE_DELAY:
		ani = directionX == Direction::Right ? GHOST_ANI_DELAY_RIGHT : GHOST_ANI_DELAY_LEFT;
		break;
	}

	if (!showingEndingEffect)
	{
		if (CGame::GetInstance()->BoundingBoxDisplayed())
		{
			RenderBoundingBox();
		}

		animationSet->at(ani)->Render(x, y);
	}
}

void CGhost::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (!showingEndingEffect)
	{
		CSettingManager* settingManager = CSettingManager::GetInstance();

		l = x;
		t = y;
		r = l + GHOST_BBOX_WIDTH;
		b = t + GHOST_BBOX_HEIGHT;
	}
}

void CGhost::TakeDamage(int damages)
{
	CEnemy::TakeDamage(damages);

	if (attacks <= 0)
	{
		Disappear();
		CGame::GetInstance()->GetPlayerData()->AddScore(CSettingManager::GetInstance()->GetIntValue("GHOST_SCORE"));
	}
	else
	{
		ShowEffect();
		SetState(GHOST_STATE_DELAY);
	}
}

int CGhost::GetDamages()
{
	return CSettingManager::GetInstance()->GetIntValue("GHOST_DAMAGES");
}

int CGhost::RandomizeDistance()
{
	return rand() % (100 + 1 - 50) + 50;
}
