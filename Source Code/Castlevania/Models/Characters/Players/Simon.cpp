#include "Simon.h"
#include "../../../Game.h"
#include "../../Misc/BigCandle.h"
#include "../../Misc/Brick.h"
#include "../../Items/BigHeart.h"
#include "../../Items/Dagger.h"
#include "../../Items/EasterEgg.h"
#include "../../Items/Item.h"
#include "../../Items/MoneyBag.h"
#include "../../Items/MorningStar.h"
#include "../../Weapons/Whip.h"

CSimon::CSimon()
{
	sitting = false;
	touchingGround = false;
	delayEndTime = -1;

	score = 0;
	hearts = 5;
	lives = 3;
	healthVolumes = 16;

	whip = new CWhip();
	whip->AddAnimation("whip_level_1_left");
	whip->AddAnimation("whip_level_1_right");
	whip->AddAnimation("whip_level_2_left");
	whip->AddAnimation("whip_level_2_right");
	whip->AddAnimation("whip_level_3_left");
	whip->AddAnimation("whip_level_3_right");

	subWeapon == nullptr;
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	vy += SIMON_GRAVITY * dt;

	if (this->state == SIMON_STATE_DELAY && GetTickCount() >= this->delayEndTime)
	{
		SetState(SIMON_STATE_IDLE);
	}

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

		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny);

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CBrick*>(e->obj))
			{
				touchingGround = true;

				if (nx != 0) vx = 0;
				if (ny != 0) vy = 0;
			}
			else if (dynamic_cast<CBigCandle*>(e->obj))
			{
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
			else if (dynamic_cast<CItem*>(e->obj))
			{
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
			else if (dynamic_cast<CEasterEgg*>(e->obj))
			{
				auto easterEgg = dynamic_cast<CEasterEgg*>(e->obj);

				easterEgg->ShowHiddenItem();
				easterEgg->SetVisibility(Visibility::Hidden);
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
	}

	if (state == SIMON_STATE_SIT_AND_ATTACK || state == SIMON_STATE_STAND_AND_ATTACK)
	{
		whip->SetDirection(direction);

		if (animations[GetAnimationToRender()]->GetCurrentFrame() == animations[GetAnimationToRender()]->GetTotalFrames() - 1)
		{
			for (int i = 0; i < coObjects->size(); i++)
			{
				if (dynamic_cast<CBigCandle*>(coObjects->at(i)))
				{
					auto bigCandle = dynamic_cast<CBigCandle*>(coObjects->at(i));

					if (CGame::GetInstance()->HaveCollision(this->whip, bigCandle))
					{
						bigCandle->Disappear();
					}
				}
			}
		}
	}

	for (int i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CItem*>(coObjects->at(i)))
		{
			auto item = dynamic_cast<CItem*>(coObjects->at(i));

			if (CGame::GetInstance()->HaveCollision(this, item))
			{
				if (dynamic_cast<CMorningStar*>(coObjects->at(i)))
				{
					SetState(SIMON_STATE_DELAY);
					this->whip->Upgrade();
				}
				else if (dynamic_cast<CBigHeart*>(coObjects->at(i)))
				{
					auto heart = dynamic_cast<CBigHeart*>(coObjects->at(i));
					AddHeart();
				}
				else if (dynamic_cast<CMoneyBag*>(coObjects->at(i)))
				{
					auto moneyBag = dynamic_cast<CMoneyBag*>(coObjects->at(i));
					AddScore(moneyBag->GetAmount());
				}
				else if (dynamic_cast<CDagger*>(coObjects->at(i)))
				{
					SetSubWeaponType("dagger");
				}

				item->Disappear();
			}
		}
	}
}

void CSimon::Render()
{
	int alpha = 255;
	animations[GetAnimationToRender()]->Render(x, y, alpha);
}

void CSimon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_WALK:
		vx = direction == Direction::Right ? SIMON_WALK_SPEED : -SIMON_WALK_SPEED;
		break;

	case SIMON_STATE_JUMP:
		if (touchingGround)
		{
			vy = -SIMON_JUMP_SPEED;
			touchingGround = false;
		}

		break;

	case SIMON_STATE_SIT:
		vx = 0;
		sitting = true;
		break;

	case SIMON_STATE_SIT_AND_ATTACK:
		vx = 0;
		animations[GetAnimationToRender()]->SetStartTime(GetTickCount());
		ResetAnimations();
		break;

	case SIMON_STATE_STAND_AND_ATTACK:
		vx = 0;
		animations[GetAnimationToRender()]->SetStartTime(GetTickCount());
		ResetAnimations();
		break;

	case SIMON_STATE_DELAY:
		vx = 0;
		delayEndTime = GetTickCount() + SIMON_DELAY_TIMEOUT;
		break;

	case SIMON_STATE_IDLE:
		vx = 0;
		touchingGround = true;
		sitting = false;
		ResetAnimations();
		break;

	case SIMON_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	}
}

void CSimon::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + SIMON_BBOX_WIDTH;
	bottom = y + SIMON_BBOX_HEIGHT;
}

int CSimon::GetAnimationToRender()
{
	int ani;

	if (state == SIMON_STATE_WALK)
	{
		if (direction == Direction::Right)
		{
			ani = SIMON_ANI_WALK_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_WALK_LEFT;
		}
	}
	else if (state == SIMON_STATE_SIT)
	{
		if (direction == Direction::Right)
		{
			ani = SIMON_ANI_SIT_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_SIT_LEFT;
		}
	}
	else if (state == SIMON_STATE_JUMP)
	{
		if (direction == Direction::Right)
		{
			ani = SIMON_ANI_JUMP_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_JUMP_LEFT;
		}
	}
	else if (state == SIMON_STATE_SIT_AND_ATTACK)
	{
		whip->SetPosition(x - 90, y + 14);

		if (direction == Direction::Right)
		{
			ani = SIMON_ANI_SIT_RIGHT_AND_ATTACK;
		}
		else
		{
			ani = SIMON_ANI_SIT_LEFT_AND_ATTACK;
		}

		whip->Render();
	}
	else if (state == SIMON_STATE_STAND_AND_ATTACK)
	{
		whip->SetPosition(x - 90, y);

		if (direction == Direction::Right)
		{
			ani = SIMON_ANI_STAND_RIGHT_AND_ATTACK;
		}
		else
		{
			ani = SIMON_ANI_STAND_LEFT_AND_ATTACK;
		}

		whip->Render();
	}
	else if (state == SIMON_STATE_DELAY)
	{
		if (direction == Direction::Right)
		{
			ani = SIMON_ANI_DELAY_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_DELAY_LEFT;
		}
	}
	else if (state == SIMON_STATE_DIE)
	{
		if (direction == Direction::Right)
		{
			ani = SIMON_ANI_DIE_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_DIE_LEFT;
		}
	}
	else
	{
		if (direction == Direction::Right)
		{
			ani = SIMON_ANI_IDLE_RIGHT;
		}
		else
		{
			ani = SIMON_ANI_IDLE_LEFT;
		}
	}

	return ani;
}

void CSimon::ResetAnimations()
{
	CGameObject::ResetAnimations();

	whip->ResetAnimations();
}

bool CSimon::Sitting()
{
	return this->sitting;
}

bool CSimon::TouchingGround()
{
	return this->touchingGround;
}

void CSimon::AddScore(int addedScore)
{
	this->score += addedScore;
}

int CSimon::GetScore()
{
	return this->score;
}

void CSimon::AddHeart()
{
	this->hearts += 5;
}

int CSimon::GetHearts()
{
	return this->hearts;
}

void CSimon::AddLife()
{
	this->lives++;
}

int CSimon::GetLives()
{
	return this->lives;
}

void CSimon::SetSubWeapon(CWeapon* weapon)
{
	this->subWeapon = weapon;
}

CWeapon* CSimon::GetSubWeapon()
{
	return this->subWeapon;
}

void CSimon::TakeDamage(int volumes)
{
	this->healthVolumes -= volumes;
}

int CSimon::getHealthVolumes()
{
	return this->healthVolumes;
}

void CSimon::SetSubWeaponType(string type)
{
	this->subWeaponType = type;
}

string CSimon::GetSubWeaponType()
{
	return this->subWeaponType;
}
