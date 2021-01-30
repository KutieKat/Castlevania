#include "Fleamen.h"
#include "../../../Game.h"
#include "../../Misc/Brick.h"
#include "../../Misc/TopStair.h"
#include "../../Misc/BottomStair.h"
#include "../../Misc/BreakableBrick.h"
#include "../../Weapons/Fireball.h"

CFleamen::CFleamen(CSimon* simon)
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	this->simon = simon;
	this->mustInArea = true;
	this->areaRadiusX = settingManager->GetIntValue("FLEAMEN_AREA_RADIUS_X");
	this->areaRadiusY = settingManager->GetIntValue("FLEAMEN_AREA_RADIUS_Y");
	this->jumpingCounter = 0;
	this->nextAttackingTime = GetTickCount() + settingManager->GetIntValue("FLEAMEN_ATTACKING_INTERVAL");

	SetAnimationSet("fleamen");
	SetState(settingManager->GetIntValue("FLEAMEN_INITIAL_STATE"));
}

void CFleamen::SetState(int state)
{
	CGameObject::SetState(state);
	CSettingManager* settingManager = CSettingManager::GetInstance();

	switch (state)
	{
	case FLEAMEN_STATE_IDLE:
		vx = vy = 0;
		break;

	case FLEAMEN_STATE_JUMP_AROUND:
		vx = simon->x < x ? -settingManager->GetFloatValue("FLEAMEN_JUMP_SPEED_X") : settingManager->GetFloatValue("FLEAMEN_JUMP_SPEED_X");	
		vy = -settingManager->GetFloatValue("FLEAMEN_JUMP_SPEED_Y");
		break;
	}
}

void CFleamen::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CSettingManager* settingManager = CSettingManager::GetInstance();

	bool softPaused = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->SoftPaused();

	if (softPaused) return;

	vy += settingManager->GetFloatValue("FLEAMEN_GRAVITY") * dt;

	directionX = x > simon->x ? Direction::Left : Direction::Right;

	if (state == FLEAMEN_STATE_JUMP_AROUND)
	{
		jumpingCounter += 1;

		if (jumpingCounter % 100 == 0)
		{
			SetState(FLEAMEN_STATE_JUMP_AROUND);
		}

		if (y < settingManager->GetIntValue("SCREEN_HEIGHT") / 2)
		{
			Attack();
		}
	}

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (coObjects) CalcPotentialCollisions(coObjects, coEvents);

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

			if (dynamic_cast<CBrick*>(e->obj))
			{
				auto brick = dynamic_cast<CBrick*>(e->obj);

				//if (brick->isGround)
				//{
					if (state == FLEAMEN_STATE_JUMP_AROUND)
					{
						if (e->ny != 0 && rdy < 25) vx = 0;
					}
					else
					{
						if (e->ny != 0) vy = 0;
					}
				//}
				//else
				//{
					if (e->nx != 0) vx = -vx;
				//}
			}
			else if (dynamic_cast<CTopStair*>(e->obj))
			{
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
			else if (dynamic_cast<CBottomStair*>(e->obj))
			{
				y -= min_ty * dy + ny * 0.4f;

				if (e->nx != 0) x += dx;
				if (e->ny < 0) y += dy;
			}
			else if (dynamic_cast<CBreakableBrick*>(e->obj))
			{
				if (e->nx != 0) vx = -vx;
			}
			else if (dynamic_cast<CEnemy*>(e->obj))
			{
				y -= min_ty * dy + ny * 0.4f;

				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
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

void CFleamen::Render()
{
	int ani;

	switch (state)
	{
	case FLEAMEN_STATE_IDLE:
		ani = directionX == Direction::Right ? FLEAMEN_ANI_IDLE_RIGHT : FLEAMEN_ANI_IDLE_LEFT;
		break;

	case FLEAMEN_STATE_JUMP_AROUND:
		ani = directionX == Direction::Right ? FLEAMEN_ANI_JUMP_RIGHT : FLEAMEN_ANI_JUMP_LEFT;
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

void CFleamen::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (!showingEndingEffect)
	{
		CSettingManager* settingManager = CSettingManager::GetInstance();

		left = x;
		top = y;
		right = left + FLEAMEN_BBOX_WIDTH;
		bottom = top + FLEAMEN_BBOX_HEIGHT;
	}
}

void CFleamen::TakeDamage(int damages)
{
	CEnemy::TakeDamage(damages);

	if (attacks <= 0)
	{
		Disappear();
		CGame::GetInstance()->GetPlayerData()->AddScore(CSettingManager::GetInstance()->GetIntValue("FLEAMEN_SCORE"));
	}
	else
	{
		ShowEffect();
	}
}

void CFleamen::OnPlayerEnterArea()
{
	CEnemy::OnPlayerEnterArea();

	if (state == FLEAMEN_STATE_IDLE)
	{
		SetState(FLEAMEN_STATE_JUMP_AROUND);
	}
}

void CFleamen::Attack()
{
	if (GetTickCount() > CGame::GetInstance()->GetPauseDeltaTime() + nextAttackingTime)
	{
		float sl, st, sr, sb;
		simon->GetBoundingBox(sl, st, sr, sb);

		WFireball* fireball = new WFireball();
		fireball->SetPosition(x, y);
		fireball->SetTarget(simon->x, simon->y);

		CGrid* grid = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->GetGrid();
		CUnit* unit = new CUnit(grid, fireball, fireball->x, fireball->y);

		nextAttackingTime = GetTickCount() + CSettingManager::GetInstance()->GetIntValue("FLEAMEN_ATTACKING_INTERVAL");
	}
}

int CFleamen::GetDamages()
{
	return CSettingManager::GetInstance()->GetIntValue("FLEAMEN_DAMAGES");
}
