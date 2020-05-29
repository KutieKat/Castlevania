#include "Raven.h"
#include "../../../Game.h"
#include "../../Misc/BottomStair.h"
#include "../../Weapons/HolyWaterBottle.h"

CRaven::CRaven(CSimon* simon)
{
	this->mustInArea = true;
	this->simon = simon;
	this->areaRadius = RAVEN_AREA_RADIUS;
	this->flyingCounter = 0;
	this->attackingCounter = 0;

	SetAnimationSet("raven");
	SetState(RAVEN_STATE_IDLE);
}

void CRaven::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case RAVEN_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;

	case RAVEN_STATE_FLY:
		break;

	case RAVEN_STATE_ATTACK:
		break;
	}
}

void CRaven::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	bool softPaused = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->SoftPaused();

	if (softPaused) return;

	directionX = x >= simon->x ? Direction::Left : Direction::Right;

	if (state == RAVEN_STATE_ATTACK)
	{
		float sl, st, sr, sb;
		simon->GetBoundingBox(sl, st, sr, sb);

		rotation = atan2(simon->y + 30 - y, (x > simon->x ? sl : sr) - 30 - x);

		dx = cos(rotation) * RAVEN_ATTACK_SPEED_X * dt;
		dy = sin(rotation) * RAVEN_ATTACK_SPEED_Y * dt;

		if (y > simon->y && ((directionX == Direction::Left && x < sl) || (directionX == Direction::Right && x > sr)))
		{
			SetState(RAVEN_STATE_FLY);
		}
	}

	if (state == RAVEN_STATE_FLY)
	{
		attackingCounter += 1;
		flyingCounter += 1;

		if (attackingCounter % 300 == 0)
		{
			Attack();
		}

		if (flyingCounter % 30 == 0)
		{
			if (x < simon->x)
			{
				if (x < simon->x - RAVEN_TO_SIMON_DISTANCE_X)
				{
					vx = RAVEN_FLY_SPEED_X;
				}
				else
				{
					vx = 0;
				}
			}
			else
			{
				if (x > simon->x + SIMON_BBOX_WIDTH + RAVEN_TO_SIMON_DISTANCE_X)
				{
					vx = -RAVEN_FLY_SPEED_X;
				}
				else
				{
					vx = 0;
				}
			}

			if (y == simon->y - RAVEN_TO_SIMON_DISTANCE_Y)
			{
				vy = 0;
			}
			else if (y < simon->y - RAVEN_TO_SIMON_DISTANCE_Y)
			{
				vy = RAVEN_FLY_SPEED_Y;
			}
			else
			{
				vy = -RAVEN_FLY_SPEED_Y;
			}
		}

		dx = vx * dt;
		dy = vy * dt;
	}

	x += dx;
	y += dy;
}

void CRaven::Render()
{
	int ani;

	switch (state)
	{
	case RAVEN_STATE_IDLE:
		ani = directionX == Direction::Right ? RAVEN_ANI_IDLE_RIGHT : RAVEN_ANI_IDLE_LEFT;
		break;

	case RAVEN_STATE_ATTACK:
	case RAVEN_STATE_FLY:
		ani = directionX == Direction::Right ? RAVEN_ANI_FLY_RIGHT : RAVEN_ANI_FLY_LEFT;
		break;
	}

	if (!showingEndingEffect)
	{
		animationSet->at(ani)->Render(x, y);
	}
}

void CRaven::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (!showingEndingEffect)
	{
		l = x;
		t = y;
		r = l + RAVEN_BBOX_WIDTH;
		b = t + RAVEN_BBOX_HEIGHT;
	}
}

void CRaven::TakeDamage(int damages)
{
	CEnemy::TakeDamage(damages);

	if (attacks <= 0)
	{
		Disappear();
		CGame::GetInstance()->GetPlayerData()->AddScore(RAVEN_SCORE);
	}
	else
	{
		ShowEffect();
	}
}

void CRaven::Attack()
{
	SetState(RAVEN_STATE_ATTACK);
}

void CRaven::OnPlayerEnterArea()
{
	bool softPaused = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->SoftPaused();

	if (softPaused) return;

	if (state == RAVEN_STATE_IDLE)
	{
		SetState(RAVEN_STATE_FLY);
	}
}
