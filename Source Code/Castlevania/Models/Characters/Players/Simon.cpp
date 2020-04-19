﻿#include "Simon.h"
#include "../../../Game.h"
#include "../../Misc/BigCandle.h"
#include "../../Misc/Ground.h"
#include "../../Misc/Door.h"
#include "../../Misc/DoorWall.h"
#include "../../Items/BigHeart.h"
#include "../../Items/Dagger.h"
#include "../../Items/EasterEgg.h"
#include "../../Items/MoneyBag.h"
#include "../../Items/MorningStar.h"
#include "../../Weapons/Whip.h"
#include "../../Weapons/WDagger.h"
#include "../../../Utilities/Debug.h"
#include "../../../Utilities/SafeDelete.h"

CSimon::CSimon()
{
	sitting = false;
	touchingGround = false;
	delayEndTime = -1;
	switchSceneTime = -1;

	whip = new CWhip();

	subWeapon == nullptr;

	SetAnimationSet("simon");
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	if (subWeapon)
	{
		float sX, sY;

		subWeapon->Update(dt, coObjects);
		subWeapon->GetPosition(sX, sY);

		if (sX > CGame::GetInstance()->GetCamera()->GetRight())
		{
			SAFE_DELETE(subWeapon);
		}
	}

	if (state != SIMON_STATE_AUTO_WALK && state != SIMON_STATE_CUT_SCENE_AUTO_WALK && state != SIMON_STATE_WATCH)
	{
		vy += SIMON_GRAVITY * dt;
	}

	if (this->state == SIMON_STATE_DELAY && GetTickCount() >= this->delayEndTime)
	{
		SetState(SIMON_STATE_IDLE);
	}

	if (state == SIMON_STATE_CUT_SCENE_AUTO_WALK && x <= (SCREEN_WIDTH / 2) - 50)
	{
		SetState(SIMON_STATE_WATCH);
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
				touchingGround = true;

				if (state == SIMON_STATE_STAND_AND_ATTACK)
				{
					vx = 0;
				}

				if (ny != 0) vy = 0;
			}
			else if (e->obj->ComeThrough() || e->obj->IsItem())
			{
				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
			else if (dynamic_cast<CEasterEgg*>(e->obj))
			{
				auto easterEgg = dynamic_cast<CEasterEgg*>(e->obj);

				if (state != SIMON_STATE_AUTO_WALK)
				{
					easterEgg->ShowHiddenItem();
					easterEgg->SetVisibility(Visibility::Hidden);
				}

				x += dx;
			}
			else if (dynamic_cast<CDoor*>(e->obj))
			{
				auto door = dynamic_cast<CDoor*>(e->obj);

				if (e->nx < 0)
				{
					door->GetHiddenItem()->SetVisibility(Visibility::Visible);
					door->SetVisibility(Visibility::Hidden);

					SetState(SIMON_STATE_AUTO_WALK);
					CGame::GetInstance()->GetTimer()->Stop();
				}
				else
				{
					x += dx;
				}

				if (e->ny != 0)
				{
					y += dy;
				}
			}
		}
	}

	if (state == SIMON_STATE_SIT_AND_ATTACK || state == SIMON_STATE_STAND_AND_ATTACK)
	{
		UpdateWhip();

		if (animationSet->at(GetAnimationToRender())->GetCurrentFrame() == animationSet->at(GetAnimationToRender())->GetTotalFrames() - 1)
		{
			for (int i = 0; i < coObjects->size(); i++)
			{
				if (dynamic_cast<CBigCandle*>(coObjects->at(i)))
				{
					auto bigCandle = dynamic_cast<CBigCandle*>(coObjects->at(i));

					float cCenterX, cLeft, cTop, cRight, cBottom;
					float sCenterX, sLeft, sTop, sRight, sBottom;
					float wCenterY, wLeft, wTop, wRight, wBottom;

					GetBoundingBox(sLeft, sTop, sRight, sBottom);
					bigCandle->GetBoundingBox(cLeft, cTop, cRight, cBottom);
					this->whip->GetBoundingBox(wLeft, wTop, wRight, wBottom);

					cCenterX = cLeft + (cRight - cLeft) / 2;
					sCenterX = sLeft + (sRight - sLeft) / 2;
					wCenterY = wTop + (wBottom - wTop) / 2;

					if (CGame::GetInstance()->HaveCollision(this->whip, bigCandle))
					{
						if (direction == Direction::Right && wCenterY >= cTop && sCenterX < cCenterX && wRight >= cLeft)
						{
							bigCandle->Disappear();
						}
						else if (direction == Direction::Left && wCenterY >= cTop && sCenterX > cCenterX && wLeft <= cRight)
						{
							bigCandle->Disappear();
						}
					}
				}
			}
		}
	}

	if (state == SIMON_STATE_STAND_AND_THROW)
	{
		if (animationSet->at(GetAnimationToRender())->GetCurrentFrame() == animationSet->at(GetAnimationToRender())->GetTotalFrames() - 1)
		{
			InitSubWeapon();
		}
	}

	if (switchSceneTime != -1 && GetTickCount() > switchSceneTime)
	{
		CGame* game = CGame::GetInstance();

		switchSceneTime = -1;
		game->SwitchScene(game->GetCurrentScene()->GetNextSceneId());
	}

	if (coObjects)
	{
		for (int i = 0; i < coObjects->size(); i++)
		{
			auto item = coObjects->at(i);

			if (item->IsItem() && CGame::GetInstance()->HaveCollision(this, item))
			{
				if (dynamic_cast<CMorningStar*>(item))
				{
					SetState(SIMON_STATE_DELAY);
					this->whip->Upgrade();
				}
				else if (dynamic_cast<CBigHeart*>(item))
				{
					CGame::GetInstance()->AddHeart(5);
				}
				else if (dynamic_cast<CDagger*>(item))
				{
					CGame::GetInstance()->SetSubWeaponType("dagger");
				}
				else if (dynamic_cast<CMoneyBag*>(item))
				{
					auto moneyBag = dynamic_cast<CMoneyBag*>(coObjects->at(i));
					CGame::GetInstance()->AddScore(moneyBag->GetValue());
				}

				item->Disappear();
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
	}
}

void CSimon::Render()
{
	animationSet->at(GetAnimationToRender())->Render(x, y);

	if (whip && (state == SIMON_STATE_SIT_AND_ATTACK || state == SIMON_STATE_STAND_AND_ATTACK))
	{
		whip->Render();
	}

	if (subWeapon)
	{
		subWeapon->Render();
	}
}

void CSimon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_WALK:
		vx = direction == Direction::Right ? SIMON_WALK_SPEED : -SIMON_WALK_SPEED;
		break;

	case SIMON_STATE_AUTO_WALK:
		direction = Direction::Right;
		vx = SIMON_WALK_SPEED;
		vy = 0;

		if (switchSceneTime == -1)
		{
			switchSceneTime = GetTickCount() + 2000;
		}

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

	case SIMON_STATE_STAND_AND_THROW:
		if (CGame::GetInstance()->GetHearts() > 0)
		{
			CGame::GetInstance()->AddHeart(-1);
		}

	case SIMON_STATE_SIT_AND_ATTACK:
	case SIMON_STATE_STAND_AND_ATTACK:
		if (touchingGround)
		{
			vx = 0;
		}

		animationSet->at(GetAnimationToRender())->SetStartTime(GetTickCount());
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

	case SIMON_STATE_CUT_SCENE_AUTO_WALK:
		direction = Direction::Left;
		vx = -SIMON_SLOW_WALK_SPEED;
		break;

	case SIMON_STATE_DIE:
	case SIMON_STATE_WATCH:
		vx = 0;

		if (switchSceneTime == -1)
		{
			switchSceneTime = GetTickCount() + 3000;
		}

		break;
	}
}

void CSimon::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + SIMON_BBOX_WIDTH;
	bottom = top + SIMON_BBOX_HEIGHT;
}

int CSimon::GetAnimationToRender()
{
	int ani;

	switch (state)
	{
	case SIMON_STATE_WALK:
	case SIMON_STATE_AUTO_WALK:
	case SIMON_STATE_CUT_SCENE_AUTO_WALK:
		ani = direction == Direction::Right ? SIMON_ANI_WALK_RIGHT : SIMON_ANI_WALK_LEFT;
		break;

	case SIMON_STATE_WATCH:
		ani = SIMON_ANI_WATCH;
		break;

	case SIMON_STATE_SIT:
		ani = direction == Direction::Right ? SIMON_ANI_SIT_RIGHT : SIMON_ANI_SIT_LEFT;
		break;

	case SIMON_STATE_JUMP:
		ani = direction == Direction::Right ? SIMON_ANI_JUMP_RIGHT : SIMON_ANI_JUMP_LEFT;
		break;

	case SIMON_STATE_SIT_AND_ATTACK:
		ani = direction == Direction::Right ? SIMON_ANI_SIT_RIGHT_AND_ATTACK : SIMON_ANI_SIT_LEFT_AND_ATTACK;
		break;

	case SIMON_STATE_STAND_AND_ATTACK:
		ani = direction == Direction::Right ? SIMON_ANI_STAND_RIGHT_AND_ATTACK : SIMON_ANI_STAND_LEFT_AND_ATTACK;
		break;

	case SIMON_STATE_DELAY:
		ani = direction == Direction::Right ? SIMON_ANI_DELAY_RIGHT : SIMON_ANI_DELAY_LEFT;
		break;

	case SIMON_STATE_DIE:
		ani = direction == Direction::Right ? SIMON_ANI_DIE_RIGHT : SIMON_ANI_DIE_LEFT;
		break;

	case SIMON_STATE_STAND_AND_THROW:
		ani = direction == Direction::Right ? SIMON_ANI_STAND_RIGHT_AND_ATTACK : SIMON_ANI_STAND_LEFT_AND_ATTACK;
		break;

	default:
		ani = direction == Direction::Right ? SIMON_ANI_IDLE_RIGHT : SIMON_ANI_IDLE_LEFT;
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

bool CSimon::Up()
{
	return this->up;
}

void CSimon::SetUp(bool up)
{
	this->up = up;
}

void CSimon::InitSubWeapon()
{
	SAFE_DELETE(subWeapon);

	subWeapon = new WDagger();
	subWeapon->SetDirection(direction);
	subWeapon->SetPosition(direction == Direction::Right ? x + 40.0f : x - 20.0f, this->sitting ? y + 8.0f : y + 16.0f);
}

void CSimon::UpdateWhip()
{
	whip->SetDirection(direction);

	switch (state)
	{
	case SIMON_STATE_SIT_AND_ATTACK:
		whip->SetPosition(direction == Direction::Right ? x - 10 : whip->GetLevel() == 3 ? x - 70 : x - 40, y + 14);
		break;

	case SIMON_STATE_STAND_AND_ATTACK:
		whip->SetPosition(direction == Direction::Right ? x - 10 : whip->GetLevel() == 3 ? x - 70 : x - 40, y);
		break;
	}
}

CSimon::~CSimon()
{
	SAFE_DELETE(whip);
	SAFE_DELETE(subWeapon);
}