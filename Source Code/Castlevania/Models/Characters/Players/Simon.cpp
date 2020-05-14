#include "Simon.h"
#include "../../../Game.h"
#include "../../Characters/Enemies/RedBat.h"
#include "../../Misc/Ground.h"
#include "../../Misc/Brick.h"
#include "../../Misc/TopStair.h"
#include "../../Misc/BottomStair.h"
#include "../../Misc/NextScene.h"
#include "../../Misc/PreviousScene.h"
#include "../../Misc/MovingBar.h"
#include "../../Misc/BreakableBrick.h"
#include "../../Items/BigHeart.h"
#include "../../Items/Dagger.h"
#include "../../Items/EasterEgg.h"
#include "../../Items/MoneyBag.h"
#include "../../Items/MorningStar.h"
#include "../../Items/RedMoneyBag.h"
#include "../../Items/PurpleMoneyBag.h"
#include "../../Items/WhiteMoneyBag.h"
#include "../../Items/Crown.h"
#include "../../Items/Boomerang.h"
#include "../../Items/SmallHeart.h"
#include "../../Items/DoubleShot.h"
#include "../../Weapons/Whip.h"
#include "../../Weapons/WDagger.h"
#include "../../Weapons/WBoomerang.h"
#include "../../../Utilities/Debug.h"
#include "../../../Utilities/SafeDelete.h"

CSimon::CSimon()
{
	invisible = false;
	sitting = false;
	touchingGround = false;
	lastFrameShown = false;
	standingToWatch = false;
	onStair = false;
	atTopStair = false;
	atBottomStair = false;
	onBar = false;

	delayEndTime = -1;
	switchSceneTime = -1;
	waitingTime = -1;
	invisibleTimeout = -1;
	waitingTime = DEFAULT_WAITING_TIME;

	whip = new CWhip(this);
	subWeapon == nullptr;

	SetAnimationSet("simon");
}

void CSimon::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	HandleGravity();

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
				if (e->ny < 0)
				{
					touchingGround = true;
					onBar = false;
				}

				if (state == SIMON_STATE_STAND_AND_ATTACK)
				{
					vx = 0;
				}

				if (e->nx != 0) vx = 0;
				if (e->ny != 0) vy = 0;
			}
			else if (dynamic_cast<CBrick*>(e->obj))
			{
				auto brick = dynamic_cast<CBrick*>(e->obj);

				if (onStair)
				{
					x += dx;
					y += dy;
				}
				else
				{
					if (brick->isGround)
					{
						if (e->ny < 0)
						{
							touchingGround = true;
							onStair = false;
							onBar = false;
						}

						if (state == SIMON_STATE_STAND_AND_ATTACK)
						{
							vx = 0;
						}

						if (e->ny != 0) vy = 0;
					}
					else
					{
						if (e->nx != 0) vx = 0;
					}
				}
			}
			else if (dynamic_cast<CBreakableBrick*>(e->obj))
			{
				auto brick = dynamic_cast<CBreakableBrick*>(e->obj);

				if (onStair)
				{
					x += dx;
					y += dy;
				}
				else
				{
					if (brick->isGround)
					{
						if (e->ny < 0)
						{
							touchingGround = true;
							onStair = false;
							onBar = false;
						}

						if (state == SIMON_STATE_STAND_AND_ATTACK)
						{
							vx = 0;
						}

						if (e->ny != 0) vy = 0;
					}
					else
					{
						if (e->nx != 0) vx = 0;
					}
				}
			}
			else if (e->obj->isItem)
			{
				y -= min_ty * dy + ny * 0.4f;

				if (e->nx != 0) x += dx;
				if (e->ny < 0) y += dy;

				if (dynamic_cast<CSmallHeart*>(e->obj))
				{
					if (e->ny > 0) y += dy;
				}
			}
			else if (dynamic_cast<CMovingBar*>(e->obj))
			{
				if (e->ny < 0)
				{
					touchingGround = true;
					onBar = true;

					vx = e->obj->vx;
				}
			}
			else if (dynamic_cast<CEasterEgg*>(e->obj))
			{
				auto easterEgg = dynamic_cast<CEasterEgg*>(e->obj);

				easterEgg->ShowHiddenItem();
				easterEgg->removable = true;

				x += dx;
			}
			else if (dynamic_cast<CNextScene*>(e->obj))
			{
				auto nextScene = dynamic_cast<CNextScene*>(e->obj);

				if (onStair == nextScene->playerMustBeOnStair)
				{
					if ((nextScene->sceneDirection == Direction::Right && (e->nx < 0 || e->ny < 0)) || (nextScene->sceneDirection == Direction::Left && (e->nx > 0 || e->ny < 0)))
					{
						CGame* game = CGame::GetInstance();

						game->GetTimer()->Pause();
						game->GetSceneManager()->SwitchScene(game->GetSceneManager()->GetNextSceneId());
						
						nextScene->removable = true;
						break;
					}

					x += dx;
					y += dy;
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
			else if (dynamic_cast<CPreviousScene*>(e->obj))
			{
				auto previousScene = dynamic_cast<CPreviousScene*>(e->obj);

				if (onStair == previousScene->playerMustBeOnStair)
				{
					if ((previousScene->sceneDirection == Direction::Right && (e->nx < 0 || e->ny < 0)) || (previousScene->sceneDirection == Direction::Left && (e->nx > 0 || e->ny < 0)))
					{
						CGame* game = CGame::GetInstance();

						game->GetTimer()->Pause();
						game->GetSceneManager()->SwitchScene(game->GetSceneManager()->GetPreviousSceneId());
						
						previousScene->removable = true;
						break;
					}

					x += dx;
					y += dy;
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
			else if (dynamic_cast<CBottomStair*>(e->obj))
			{
				auto stair = dynamic_cast<CBottomStair*>(e->obj);

				if (onStair)
				{
					onStair = false;
					SetState(SIMON_STATE_IDLE);
				}

				stairDirectionX = stair->directionX;
				stairDirectionY = stair->directionY;

				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
			else if (dynamic_cast<CTopStair*>(e->obj))
			{
				auto stair = dynamic_cast<CTopStair*>(e->obj);

				if (onStair)
				{
					onStair = false;
					SetState(SIMON_STATE_IDLE);
				}

				stairDirectionX = stair->directionX;
				stairDirectionY = stair->directionY;

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

	HandleDelay();
	HandleAttackWithWhip(coObjects);
	HandleAttackWithSubWeapon(coObjects);
	HandleCollisionObjects(coObjects);
	HandleSwitchScene();
	HandleInvisibility();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
	}
}

void CSimon::Render()
{
	animationSet->at(GetAnimationToRender())->Render(x, y, invisible ? 150 : 255);

	RenderWhip();
}

void CSimon::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case SIMON_STATE_WALK:
		vx = directionX == Direction::Right ? SIMON_WALK_SPEED : -SIMON_WALK_SPEED;
		break;

	case SIMON_STATE_JUMP:
		if (touchingGround)
		{
			vy = -SIMON_JUMP_SPEED;
			touchingGround = false;
		}

		break;

	case SIMON_STATE_SIT:
		if (!onBar)
		{
			vx = 0;
		}

		sitting = true;
		break;

	case SIMON_STATE_STAND_AND_THROW:
	case SIMON_STATE_STAND_ON_STAIR_AND_THROW:
		if (CGame::GetInstance()->GetPlayerData()->GetHearts() > 0)
		{
			CGame::GetInstance()->GetPlayerData()->DecreaseHearts(1);
		}

	case SIMON_STATE_SIT_AND_ATTACK:
	case SIMON_STATE_STAND_AND_ATTACK:
		if (touchingGround)
		{
			vx = 0;
		}

		//if (!onBar)
		//{
		//	vx = 0;
		//}

		animationSet->at(GetAnimationToRender())->SetStartTime(GetTickCount());
		ResetAnimations();
		break;

	case SIMON_STATE_WALK_UPSTAIR_AND_ATTACK:
	case SIMON_STATE_WALK_DOWNSTAIR_AND_ATTACK:
		vx = 0;
		vy = 0;

		animationSet->at(GetAnimationToRender())->SetStartTime(GetTickCount());
		ResetAnimations();
		break;

	case SIMON_STATE_DELAY:
		vx = 0;
		delayEndTime = GetTickCount() + SIMON_DELAY_TIME;
		break;

	case SIMON_STATE_IDLE:
		if (!onBar)
		{
			vx = 0;
		}

		touchingGround = true;
		sitting = false;
		ResetAnimations();
		break;

	case SIMON_STATE_CUT_SCENE_AUTO_WALK:
		if (movingDirection == "rightward")
		{
			directionX = Direction::Right;
			vx = SIMON_SLOW_WALK_SPEED;
		}
		else if (movingDirection == "leftward")
		{
			directionX = Direction::Left;
			vx = -SIMON_SLOW_WALK_SPEED;
		}

		break;

	case SIMON_STATE_IDLE_ON_STAIR:
		vx = 0;
		vy = 0;
		break;

	case SIMON_STATE_WALK_DOWNSTAIR:
		if (stairDirectionX == directionX)
		{
			vx = stairDirectionX == Direction::Right ? SIMON_WALK_SPEED : -SIMON_WALK_SPEED;
		}
		else
		{
			vx = stairDirectionX == Direction::Right ? -SIMON_WALK_SPEED : SIMON_WALK_SPEED;
		}

		vy = SIMON_WALK_SPEED;
		directionY = Direction::Down;
		break;

	case SIMON_STATE_WALK_UPSTAIR:
		if (stairDirectionX == directionX)
		{
			vx = stairDirectionX == Direction::Right ? SIMON_WALK_SPEED : -SIMON_WALK_SPEED;
		}
		else
		{
			vx = stairDirectionX == Direction::Right ? -SIMON_WALK_SPEED : SIMON_WALK_SPEED;
		}

		vy = -SIMON_WALK_SPEED;
		directionY = Direction::Up;
		break;

	case SIMON_STATE_DEFLECT:
		invisible = true;
		invisibleTimeout = GetTickCount() + SIMON_INVISIBILITY_TIME;

		if (directionX == Direction::Right)
		{
			vy = -SIMON_JUMP_SPEED;
			vx = -SIMON_WALK_SPEED;
		}
		else
		{
			vy = -SIMON_JUMP_SPEED;
			vx = SIMON_WALK_SPEED;
		}

		animationSet->at(GetAnimationToRender())->SetStartTime(GetTickCount());

		break;

	case SIMON_STATE_DIE:
	case SIMON_STATE_WATCH:
		vx = 0;

		if (switchSceneTime == -1)
		{
			switchSceneTime = GetTickCount() + waitingTime;
		}

		break;
	}
}

void CSimon::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x + 15;
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
	case SIMON_STATE_CUT_SCENE_AUTO_WALK:
		ani = directionX == Direction::Right ? SIMON_ANI_WALK_RIGHT : SIMON_ANI_WALK_LEFT;
		break;

	case SIMON_STATE_IDLE_ON_STAIR:
		if (stairDirectionY == Direction::Up)
		{
			if (directionX == stairDirectionX)
			{
				ani = directionX == Direction::Right ? SIMON_ANI_IDLE_ON_STAIR_UP_RIGHT : SIMON_ANI_IDLE_ON_STAIR_UP_LEFT;
			}
			else
			{
				ani = directionX == Direction::Right ? SIMON_ANI_IDLE_ON_STAIR_DOWN_RIGHT : SIMON_ANI_IDLE_ON_STAIR_DOWN_LEFT;
			}
		}
		else
		{
			if (directionX == stairDirectionX)
			{
				ani = directionX == Direction::Right ? SIMON_ANI_IDLE_ON_STAIR_DOWN_RIGHT : SIMON_ANI_IDLE_ON_STAIR_DOWN_LEFT;
			}
			else
			{
				ani = directionX == Direction::Right ? SIMON_ANI_IDLE_ON_STAIR_UP_RIGHT : SIMON_ANI_IDLE_ON_STAIR_UP_LEFT;
			}
		}

		break;

	case SIMON_STATE_WALK_UPSTAIR:
		ani = directionX == Direction::Right ? SIMON_ANI_WALK_UPSTAIR_RIGHT : SIMON_ANI_WALK_UPSTAIR_LEFT;
		break;

	case SIMON_STATE_WALK_DOWNSTAIR:
		ani = directionX == Direction::Right ? SIMON_ANI_WALK_DOWNSTAIR_RIGHT : SIMON_ANI_WALK_DOWNSTAIR_LEFT;
		break;

	case SIMON_STATE_WATCH:
		ani = SIMON_ANI_WATCH;
		break;

	case SIMON_STATE_SIT:
		ani = directionX == Direction::Right ? SIMON_ANI_SIT_RIGHT : SIMON_ANI_SIT_LEFT;
		break;

	case SIMON_STATE_JUMP:
		ani = directionX == Direction::Right ? SIMON_ANI_JUMP_RIGHT : SIMON_ANI_JUMP_LEFT;
		break;

	case SIMON_STATE_SIT_AND_ATTACK:
		ani = directionX == Direction::Right ? SIMON_ANI_SIT_RIGHT_AND_ATTACK : SIMON_ANI_SIT_LEFT_AND_ATTACK;
		break;

	case SIMON_STATE_STAND_AND_ATTACK:
		ani = directionX == Direction::Right ? SIMON_ANI_STAND_RIGHT_AND_ATTACK : SIMON_ANI_STAND_LEFT_AND_ATTACK;
		break;

	case SIMON_STATE_DELAY:
		ani = directionX == Direction::Right ? SIMON_ANI_DELAY_RIGHT : SIMON_ANI_DELAY_LEFT;
		break;

	case SIMON_STATE_DIE:
		ani = directionX == Direction::Right ? SIMON_ANI_DIE_RIGHT : SIMON_ANI_DIE_LEFT;
		break;

	case SIMON_STATE_STAND_AND_THROW:
		ani = directionX == Direction::Right ? SIMON_ANI_STAND_RIGHT_AND_ATTACK : SIMON_ANI_STAND_LEFT_AND_ATTACK;
		break;

	case SIMON_STATE_WALK_UPSTAIR_AND_ATTACK:
		ani = directionX == Direction::Right ? SIMON_ANI_WALK_UPSTAIR_RIGHT_AND_ATTACK : SIMON_ANI_WALK_UPSTAIR_LEFT_AND_ATTACK;
		break;

	case SIMON_STATE_WALK_DOWNSTAIR_AND_ATTACK:
		ani = directionX == Direction::Right ? SIMON_ANI_WALK_DOWNSTAIR_RIGHT_AND_ATTACK : SIMON_ANI_WALK_DOWNSTAIR_LEFT_AND_ATTACK;
		break;

	case SIMON_STATE_DEFLECT:
		ani = directionX == Direction::Right ? SIMON_ANI_DEFLECT_LEFT : SIMON_ANI_DEFLECT_RIGHT;
		break;

	case SIMON_STATE_STAND_ON_STAIR_AND_THROW:
		if (directionY == Direction::Up)
		{
			ani = directionX == Direction::Right ? SIMON_ANI_WALK_UPSTAIR_RIGHT_AND_ATTACK : SIMON_ANI_WALK_UPSTAIR_LEFT_AND_ATTACK;
		}
		else
		{
			ani = directionX == Direction::Right ? SIMON_ANI_WALK_DOWNSTAIR_RIGHT_AND_ATTACK : SIMON_ANI_WALK_DOWNSTAIR_LEFT_AND_ATTACK;
		}
		break;

	default:
		ani = directionX == Direction::Right ? SIMON_ANI_IDLE_RIGHT : SIMON_ANI_IDLE_LEFT;
	}

	return ani;
}

void CSimon::ResetAnimations()
{
	CGameObject::ResetAnimations();

	lastFrameShown = false;
	whip->ResetAnimations();
}

void CSimon::HandleGravity()
{
	if (!onStair && state != SIMON_STATE_WALK_UPSTAIR && state != SIMON_STATE_CUT_SCENE_AUTO_WALK && state != SIMON_STATE_WATCH)
	{
		vy += SIMON_GRAVITY * dt;
	}
}

void CSimon::HandleDelay()
{
	if (state == SIMON_STATE_DELAY && delayEndTime != -1 && GetTickCount() >= delayEndTime && state != SIMON_STATE_DIE)
	{
		delayEndTime = -1;
		SetState(SIMON_STATE_IDLE);
	}
}

void CSimon::HandleAttackWithWhip(vector<LPGAMEOBJECT>* coObjects)
{
	if (whip && (state == SIMON_STATE_SIT_AND_ATTACK || state == SIMON_STATE_STAND_AND_ATTACK || state == SIMON_STATE_WALK_DOWNSTAIR_AND_ATTACK || state == SIMON_STATE_WALK_UPSTAIR_AND_ATTACK))
	{
		whip->directionX = directionX;

		switch (state)
		{
		case SIMON_STATE_SIT_AND_ATTACK:
			whip->SetPosition(directionX == Direction::Right ? x - 10 : CGame::GetInstance()->GetPlayerData()->GetWhipLevel() == WHIP_LEVEL_3 ? x - 70 : x - 40, y + 14);
			break;

		case SIMON_STATE_STAND_AND_ATTACK:
		case SIMON_STATE_WALK_UPSTAIR_AND_ATTACK:
		case SIMON_STATE_WALK_DOWNSTAIR_AND_ATTACK:
			whip->SetPosition(directionX == Direction::Right ? x - 10 : CGame::GetInstance()->GetPlayerData()->GetWhipLevel() == WHIP_LEVEL_3 ? x - 70 : x - 40, y);
			break;
		}

		if (lastFrameShown == false && animationSet->at(GetAnimationToRender())->ReachLastFrame())
		{
			lastFrameShown = true;

			whip->Update(dt, coObjects);
		}
	}
}

void CSimon::HandleAttackWithSubWeapon(vector<LPGAMEOBJECT>* coObjects)
{
	if (state == SIMON_STATE_STAND_AND_THROW || state == SIMON_STATE_STAND_ON_STAIR_AND_THROW)
	{
		if (lastFrameShown == false && animationSet->at(GetAnimationToRender())->ReachLastFrame())
		{
			lastFrameShown = true;

			string type = CGame::GetInstance()->GetPlayerData()->GetSubWeaponType();
			CGameObject* weapon = nullptr;

			if (type == "dagger")
			{
				weapon = new WDagger();
			}
			else if (type == "boomerang")
			{
				weapon = new WBoomerang(this);
			}

			weapon->SetDirectionX(directionX);
			weapon->SetPosition(directionX == Direction::Right ? x + 40.0f : x - 20.0f, y + 12.0f);

			CGrid* grid = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->GetGrid();
			CUnit* unit = new CUnit(grid, weapon, weapon->x, weapon->y);
		}
	}
}

void CSimon::HandleCollisionObjects(vector<LPGAMEOBJECT>* coObjects)
{
	if (coObjects && state != SIMON_STATE_DIE)
	{
		int countAtBottom = 0, countAtTop = 0;
		CGame* game = CGame::GetInstance();

		for (int i = 0; i < coObjects->size(); i++)
		{
			auto object = coObjects->at(i);

			if (game->HaveCollision(this, object))
			{
				if (object->isItem)
				{
					HandleCollisionWithItems(object);
				}
				else if (dynamic_cast<CEnemy*>(object))
				{
					HandleCollisionWithEnemies(object);
				}
				else if (dynamic_cast<CTopStair*>(object))
				{
					countAtTop++;
				}
				else if (dynamic_cast<CBottomStair*>(object))
				{
					countAtBottom++;
				}
			}
			else if (dynamic_cast<CEnemy*>(object))
			{
				auto enemy = dynamic_cast<CEnemy*>(object);

				if (CEnemy::IsPlayerNearby(enemy->x, enemy->y, x, y, enemy->GetAreaRadius()))
				{
					enemy->OnPlayerEnterArea();
				}
			}
		}

		atTopStair = touchingGround && countAtTop > 0;
		atBottomStair = touchingGround && countAtBottom > 0;
	}
}

void CSimon::HandleCollisionWithItems(CGameObject* item)
{
	CPlayerData* playerData = CGame::GetInstance()->GetPlayerData();

	if (dynamic_cast<CMorningStar*>(item))
	{
		SetState(SIMON_STATE_DELAY);

		playerData->SetWhipLevel(playerData->GetWhipLevel() + 1);
	}
	else if (dynamic_cast<CBigHeart*>(item))
	{
		playerData->AddHearts(BIG_HEART_HEARTS);
	}
	else if (dynamic_cast<CSmallHeart*>(item))
	{
		playerData->AddHearts(SMALL_HEART_HEARTS);
	}
	else if (dynamic_cast<CDagger*>(item))
	{
		playerData->SetSubWeaponType("dagger");
	}
	else if (dynamic_cast<CMoneyBag*>(item))
	{
		playerData->AddScore(MONEY_BAG_SCORE);
	}
	else if (dynamic_cast<CRedMoneyBag*>(item))
	{
		playerData->AddScore(RED_MONEY_BAG_SCORE);
	}
	else if (dynamic_cast<CPurpleMoneyBag*>(item))
	{
		playerData->AddScore(PURPLE_MONEY_BAG_SCORE);
	}
	else if (dynamic_cast<CWhiteMoneyBag*>(item))
	{
		playerData->AddScore(WHITE_MONEY_BAG_SCORE);
	}
	else if (dynamic_cast<CCrown*>(item))
	{
		playerData->AddScore(CROWN_SCORE);
	}
	else if (dynamic_cast<CBoomerang*>(item))
	{
		playerData->SetSubWeaponType("boomerang");
	}
	else if (dynamic_cast<CDoubleShot*>(item))
	{
		playerData->SetWhipPower(WHIP_DOUBLE_POWER);
	}

	item->Disappear();
}

void CSimon::HandleCollisionWithEnemies(CGameObject* item)
{
	CPlayerData* playerData = CGame::GetInstance()->GetPlayerData();

	if (!invisible)
	{
		playerData->DecreaseHealthVolumes(1);
		SetState(SIMON_STATE_DEFLECT);
	}
}

void CSimon::HandleSwitchScene()
{
	CGame* game = CGame::GetInstance();

	if (state == SIMON_STATE_CUT_SCENE_AUTO_WALK)
	{
		if (switchScenePosition == "right")
		{
			if (x >= SCREEN_WIDTH)
			{
				if (standingToWatch)
				{
					SetState(SIMON_STATE_WATCH);
				}
				else
				{
					if (switchSceneTime == -1)
					{
						switchSceneTime = GetTickCount() + waitingTime;
					}
				}
			}
		}
		else if (switchScenePosition == "center")
		{
			if (x <= (SCREEN_WIDTH / 2) - 50)
			{
				if (standingToWatch)
				{
					SetState(SIMON_STATE_WATCH);
				}
				else
				{
					if (switchSceneTime == -1)
					{
						switchSceneTime = GetTickCount() + waitingTime;
					}
				}
			}
		}
		else
		{
			if (x <= 0)
			{
				if (standingToWatch)
				{
					SetState(SIMON_STATE_WATCH);
				}
				else
				{
					if (switchSceneTime == -1)
					{
						switchSceneTime = GetTickCount() + waitingTime;
					}
				}
			}
		}
	}

	if (switchSceneTime != -1 && GetTickCount() > switchSceneTime)
	{
		switchSceneTime = -1;

		if (state != SIMON_STATE_DIE)
		{
			game->GetSceneManager()->SwitchScene(game->GetSceneManager()->GetNextSceneId());
		}
		else
		{
			game->GetSceneManager()->SwitchScene(game->GetSceneManager()->GetFirstSceneId());
		}
	}
}

void CSimon::HandleInvisibility()
{
	if (invisible && invisibleTimeout != -1 && GetTickCount() >= invisibleTimeout)
	{
		invisible = false;
		invisibleTimeout = -1;
	}
}

void CSimon::RenderWhip()
{
	if (whip && (state == SIMON_STATE_SIT_AND_ATTACK || state == SIMON_STATE_STAND_AND_ATTACK || state == SIMON_STATE_WALK_DOWNSTAIR_AND_ATTACK || state == SIMON_STATE_WALK_UPSTAIR_AND_ATTACK))
	{
		whip->Render();
	}
}

CSimon::~CSimon()
{
	SAFE_DELETE(whip);
	SAFE_DELETE(subWeapon);
}