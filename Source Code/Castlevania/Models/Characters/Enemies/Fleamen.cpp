#include "Fleamen.h"
#include "../../../Game.h"
#include "../../Misc/Brick.h"
#include "../../Misc/Ground.h"
#include "../../Misc/TopStair.h"
#include "../../Misc/BottomStair.h"
#include "../../Misc/BreakableBrick.h"
#include "../../Weapons/Fireball.h"

CFleamen::CFleamen(CSimon* simon)
{
	this->simon = simon;
	this->mustInArea = true;
	this->areaRadiusX = FLEAMEN_AREA_RADIUS_X;
	this->areaRadiusY = FLEAMEN_AREA_RADIUS_Y;
	this->jumpingCounter = 0;
	this->nextAttackingTime = GetTickCount() + FLEAMEN_ATTACKING_INTERVAL;

	SetAnimationSet("fleamen");
	SetState(FLEAMEN_STATE_IDLE);
}

void CFleamen::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case FLEAMEN_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;

	case FLEAMEN_STATE_JUMP_AROUND:
		vx = simon->x < x ? -FLEAMEN_JUMP_SPEED_X : FLEAMEN_JUMP_SPEED_X;	
		vy = -FLEAMEN_JUMP_SPEED_Y;
		break;
	}
}

void CFleamen::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	bool softPaused = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->SoftPaused();

	if (softPaused) return;

	vy += FLEAMEN_GRAVITY * dt;

	directionX = x > simon->x ? Direction::Left : Direction::Right;

	if (state == FLEAMEN_STATE_JUMP_AROUND)
	{
		jumpingCounter += 1;

		if (jumpingCounter % 100 == 0)
		{
			SetState(FLEAMEN_STATE_JUMP_AROUND);
		}

		if (y < SCREEN_HEIGHT / 2)
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

			if (dynamic_cast<CGround*>(e->obj))
			{
				if (state == FLEAMEN_STATE_JUMP_AROUND)
				{
					if (e->ny != 0 && rdy < 25) vx = 0;
				}
				else
				{
					if (e->ny != 0) vy = 0;
				}
			}
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				auto brick = dynamic_cast<CBrick*>(e->obj);

				if (brick->isGround)
				{
					if (state == FLEAMEN_STATE_JUMP_AROUND)
					{
						if (e->ny != 0 && rdy < 25) vx = 0;
					}
					else
					{
						if (e->ny != 0) vy = 0;
					}
				}
				else
				{
					if (e->nx != 0) vx = -vx;
				}
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
		animationSet->at(ani)->Render(x, y);
	}
}

void CFleamen::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	if (!showingEndingEffect)
	{
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
		CGame::GetInstance()->GetPlayerData()->AddScore(FLEAMEN_SCORE);
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
		fireball->SetDisplayTime(FIREBALL_DISPLAY_TIME);

		if (x > simon->x)
		{
			fireball->SetTarget(sl, sb);
		}
		else if (x < simon->x)
		{
			fireball->SetTarget(sr, sb);
		}
		else
		{
			fireball->SetTarget(sl + (sr - sl) / 2, sb);
		}

		CGrid* grid = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->GetGrid();
		CUnit* unit = new CUnit(grid, fireball, fireball->x, fireball->y);

		nextAttackingTime = GetTickCount() + FLEAMEN_ATTACKING_INTERVAL;
	}
}