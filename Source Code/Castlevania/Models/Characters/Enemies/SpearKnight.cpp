#include "SpearKnight.h"
#include "../../../Game.h"
#include "../../Misc/BottomStair.h"
#include "../../Misc/Brick.h"
#include "../../Weapons/HolyWaterBottle.h"

CSpearKnight::CSpearKnight()
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	SetAnimationSet("spear_knight");

	attacks = settingManager->GetIntValue("SPEAR_KNIGHT_ATTACKS");
	SetState(settingManager->GetIntValue("SPEAR_KNIGHT_INITIAL_STATE"));
}

void CSpearKnight::SetState(int state)
{
	CGameObject::SetState(state);
	CSettingManager* settingManager = CSettingManager::GetInstance();

	switch (state)
	{
	case SPEAR_KNIGHT_STATE_WALK:
		vx = directionX == Direction::Right ? settingManager->GetFloatValue("SPEAR_KNIGHT_WALK_SPEED") : -settingManager->GetFloatValue("SPEAR_KNIGHT_WALK_SPEED");
		break;

	case SPEAR_KNIGHT_STATE_DELAY:
		vx = 0;
		delayTimeout = GetTickCount() + settingManager->GetIntValue("ENEMY_DELAY_TIME");
		break;
	}
}

void CSpearKnight::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CSettingManager* settingManager = CSettingManager::GetInstance();

	if (state == SPEAR_KNIGHT_STATE_DELAY && delayTimeout != -1 && GetTickCount() > delayTimeout)
	{
		if (GetTickCount() > delayTimeout + settingManager->GetIntValue("ENEMY_DELAY_PLUS_TIME"))
		{
			attacks = settingManager->GetIntValue("SPEAR_KNIGHT_ATTACKS");
			delayTimeout = -1;
		}

		SetState(SPEAR_KNIGHT_STATE_WALK);
	}

	bool softPaused = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->SoftPaused();

	if (hiddenItem && !softPaused) hiddenItem->SetPosition(x, y - SPEAR_KNIGHT_BBOX_HEIGHT * 2);
	if (softPaused) return;

	vy += settingManager->GetFloatValue("SPEAR_KNIGHT_GRAVITY") * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (coObjects) CalcPotentialCollisions(coObjects, coEvents);

	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (x <= leftBound)
		{
			directionX = Direction::Right;
			vx = -vx;
		}
		else if (x >= rightBound)
		{
			directionX = Direction::Left;
			vx = -vx;
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
				auto brick = dynamic_cast<CBrick*>(e->obj);

				//if (brick->isGround)
				//{
					if (e->ny != 0) vy = 0;
				//}
				//else
				//{
					if (e->nx != 0)
					{
						directionX = directionX == Direction::Left ? Direction::Right : Direction::Left;
						vx = -vx;
					}
				//}
			}
			else if (dynamic_cast<CBottomStair*>(e->obj))
			{
				if (e->nx != 0) x += dx;
			}
			else if (dynamic_cast<CEnemy*>(e->obj))
			{
				y -= min_ty * dy + ny * 0.4f;

				if (e->nx != 0) x += dx;
			}
			else
			{
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
		}
	}
}

void CSpearKnight::Render()
{
	int ani;

	switch (state)
	{
	case SPEAR_KNIGHT_STATE_DELAY:
		ani = directionX == Direction::Right ? SPEAR_KNIGHT_ANI_DELAY_RIGHT : SPEAR_KNIGHT_ANI_DELAY_LEFT;
		break;

	case SPEAR_KNIGHT_STATE_WALK:
		ani = directionX == Direction::Right ? SPEAR_KNIGHT_ANI_WALK_RIGHT : SPEAR_KNIGHT_ANI_WALK_LEFT;
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

void CSpearKnight::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (!showingEndingEffect)
	{
		CSettingManager* settingManager = CSettingManager::GetInstance();

		l = x;
		t = y;
		r = l + SPEAR_KNIGHT_BBOX_WIDTH;
		b = t + SPEAR_KNIGHT_BBOX_HEIGHT;
	}
}

void CSpearKnight::TakeDamage(int damages)
{
	CEnemy::TakeDamage(damages);

	if (attacks <= 0)
	{
		Disappear();
		CGame::GetInstance()->GetPlayerData()->AddScore(CSettingManager::GetInstance()->GetIntValue("SPEAR_KNIGHT_SCORE"));
	}
	else
	{
		ShowEffect();
		SetState(SPEAR_KNIGHT_STATE_DELAY);
	}
}

int CSpearKnight::GetDamages()
{
	return CSettingManager::GetInstance()->GetIntValue("SPEAR_KNIGHT_DAMAGES");
}
