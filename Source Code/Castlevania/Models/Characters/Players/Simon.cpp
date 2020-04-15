#include "Simon.h"
#include "../../../Game.h"
#include "../../Misc/BigCandle.h"
#include "../../Misc/Brick.h"
#include "../../Misc/Door.h"
#include "../../Misc/DoorWall.h"
#include "../../Items/BigHeart.h"
#include "../../Items/Dagger.h"
#include "../../Items/EasterEgg.h"
#include "../../Items/Item.h"
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

	score = 0;
	hearts = 5;
	lives = 3;
	healthVolumes = 16;

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

				if (ny != 0) vy = 0;
			}
			else if (dynamic_cast<CBigCandle*>(e->obj) || dynamic_cast<CItem*>(e->obj) || dynamic_cast<CDoorWall*>(e->obj))
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
					//door->GetDoorWall()->SetVisibility(Visibility::Visible);
					//door->SetVisibility(Visibility::Hidden);

					//SetState(SIMON_STATE_AUTO_WALK);
					//CGame::GetInstance()->GetTimer()->Stop();

					//CDebug::Info("NextSceneId=" + door->GetNextSceneId());

					CGame::GetInstance()->SwitchScene(door->GetNextSceneId());
					CGame::GetInstance()->GetTimer()->SetTime(300);
					CGame::GetInstance()->GetTimer()->Start();
				}

				if (e->ny != 0)
				{
					y += dy;
				}
			}
		}
	}

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
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
					AddHeart(5);
				}
				else if (dynamic_cast<CMoneyBag*>(coObjects->at(i)))
				{
					auto moneyBag = dynamic_cast<CMoneyBag*>(coObjects->at(i));
					AddScore(moneyBag->GetScore());
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
		if (this->hearts > 0)
		{
			this->hearts -= 1;
		}

	case SIMON_STATE_SIT_AND_ATTACK:
	case SIMON_STATE_STAND_AND_ATTACK:
		vx = 0;
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
		ani = direction == Direction::Right ? SIMON_ANI_WALK_RIGHT : SIMON_ANI_WALK_LEFT;
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

void CSimon::AddScore(int addedScore)
{
	this->score += addedScore;
}

int CSimon::GetScore()
{
	return this->score;
}

void CSimon::AddHeart(int hearts)
{
	this->hearts += hearts;
}

void CSimon::DecreaseHeart(int hearts)
{
	this->hearts -= hearts;
}

int CSimon::GetHearts()
{
	return this->hearts;
}

void CSimon::AddLife(int lives)
{
	this->lives += lives;
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