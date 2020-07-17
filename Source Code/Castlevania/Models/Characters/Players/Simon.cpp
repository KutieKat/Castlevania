#include "Simon.h"
#include "../../../Game.h"
#include "../../Characters/Enemies/RedBat.h"
#include "../../Characters/Enemies/Fleamen.h"
#include "../../Characters/Enemies/WhiteSkeleton.h"
#include "../../Characters/Bosses/PhantomBat.h"
#include "../../Characters/Enemies/Raven.h"
#include "../../Effects/Flash.h"
#include "../../Misc/Brick.h"
#include "../../Misc/TopStair.h"
#include "../../Misc/BottomStair.h"
#include "../../Misc/BiStair.h"
#include "../../Misc/NextScene.h"
#include "../../Misc/PreviousScene.h"
#include "../../Misc/MovingBar.h"
#include "../../Misc/BreakableBrick.h"
#include "../../Misc/EnemySpawnerArea.h"
#include "../../Misc/CameraLocker.h"
#include "../../Items/BigHeart.h"
#include "../../Items/Dagger.h"
#include "../../Items/Axe.h"
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
#include "../../Items/TripleShot.h"
#include "../../Items/PorkChop.h"
#include "../../Items/Rosary.h"
#include "../../Items/HolyWater.h"
#include "../../Items/Stopwatch.h"
#include "../../Items/MagicCrystal.h"
#include "../../Items/InvisibilityPotion.h"
#include "../../Weapons/Whip.h"
#include "../../Weapons/WDagger.h"
#include "../../Weapons/WBoomerang.h"
#include "../../Weapons/WAxe.h"
#include "../../Weapons/HolyWaterBottle.h"
#include "../../Weapons/WStopwatch.h"
#include "../../Weapons/Bone.h"
#include "../../Weapons/Fireball.h"
#include "../../../Scenes/PlayScene.h"
#include "../../../Utilities/Debug.h"
#include "../../../Utilities/SafeDelete.h"

CSimon::CSimon()
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	jumpable = false;
	partiallyInvisible = false;
	fullyInvisible = false;
	sitting = false;
	touchingGround = false;
	up = false;
	down = false;
	lastFrameShown = false;
	standingToWatch = false;
	onStair = false;
	atTopStair = false;
	atBottomStair = false;
	atBiStair = false;
	onBar = false;
	deflecting = false;
	switchSceneEnabled = false;
	startingEndingCounter = false;

	sittingCounter = 0;
	endingCounter = 0;
	elevation = settingManager->GetIntValue("SIMON_DEFAULT_ELEVATION");
	delayEndTime = -1;
	switchSceneTime = -1;
	invisibleTimeout = -1;
	waitingTime = settingManager->GetIntValue("DEFAULT_WAITING_TIME");

	whip = new CWhip(this);
	whip->SetVisibility(Visibility::Hidden);

	subWeapon = nullptr;

	CGrid* grid = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->GetGrid();
	CUnit* unit = new CUnit(grid, whip, x, y);

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

			if (dynamic_cast<CBrick*>(e->obj))
			{
				auto brick = dynamic_cast<CBrick*>(e->obj);

				if (onStair)
				{
					x += dx;
					y += dy;
				}
				else
				{
					//if (brick->isGround)
					//{
						if (e->ny < 0)
						{
							jumpable = true;
							touchingGround = true;
							onStair = false;
							onBar = false;

							if (state == SIMON_STATE_STAND_AND_ATTACK)
							{
								vx = 0;
							}
						}

						if (brick->canJumpThrough)
						{
							if (e->ny > 0)
							{
								if (e->ny != 0) y += dy;
							}
							else
							{
								if (e->ny != 0) vy = 0;
							}
						}
						else
						{
							if (e->ny != 0) vy = 0;
						}

						//if (e->nx != 0) vx = 0;
					//}
					//else
					//{
						//if (e->nx != 0) vx = 0;
					//}
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
							jumpable = true;
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
			else if (dynamic_cast<CEasterEgg*>(e->obj))
			{
				if (e->nx != 0) x += dx;
			}
			else if (dynamic_cast<CEnemy*>(e->obj))
			{
				y -= min_ty * dy + ny * 0.4f;

				if (e->nx != 0) x += dx;

				if (dynamic_cast<CRaven*>(e->obj) || dynamic_cast<CPhantomBat*>(e->obj))
				{
					if (e->ny < 0) y += dy;
				}
				else
				{
					if (e->ny != 0) y += dy;
				}
			}
			else if (dynamic_cast<CMovingBar*>(e->obj))
			{
				if (e->ny < 0)
				{
					jumpable = true;
					touchingGround = true;
					onBar = true;

					vx = e->obj->vx;
				}
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
			else if (dynamic_cast<CCameraLocker*>(e->obj))
			{
				CGame::GetInstance()->GetSoundManager()->StopBackgroundSounds();
				CGame::GetInstance()->GetSoundManager()->Play("boss_background_music");
				CGame::GetInstance()->GetCamera()->Lock();

				e->obj->removable = true;

				if (e->nx != 0) x += dx;
				if (e->ny != 0) y += dy;
			}
			else if (dynamic_cast<WBone*>(e->obj) || dynamic_cast<WFireball*>(e->obj))
			{
				y -= min_ty * dy + ny * 0.4f;

				if (e->nx != 0) x += dx;
				if (e->ny < 0) y += dy;
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
	HandleEndingGame();
	HandleJumpingOutOfWorld();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		delete coEvents[i];
	}
}

void CSimon::Render()
{
	int alpha;

	if (partiallyInvisible)
	{
		alpha = 150;
	}
	else if (fullyInvisible)
	{
		if (invisibleTimeout == -1)
		{
			alpha = 0;
		}
		else
		{
			if (GetTickCount() <= invisibleTimeout - 2000)
			{
				alpha = 0;
			}
			else
			{
				alpha = 100;
			}
		}
	}
	else
	{
		alpha = 255;
	}

	animationSet->at(GetAnimationToRender())->Render(x, y, alpha);

	RenderWhip();
}

void CSimon::SetState(int state)
{
	CGameObject::SetState(state);
	CSettingManager* settingManager = CSettingManager::GetInstance();

	switch (state)
	{
	case SIMON_STATE_WALK:
		vx = directionX == Direction::Right ? settingManager->GetFloatValue("SIMON_JUMP_SPEED_X") : -settingManager->GetFloatValue("SIMON_JUMP_SPEED_X");
		break;

	case SIMON_STATE_JUMP:
		if (touchingGround)
		{
			//if (jumpable)
			//{
				jumpable = false;
				touchingGround = false;

				vy = -settingManager->GetFloatValue("SIMON_JUMP_SPEED_Y");
			//}
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
	{
		CPlayerData* playerData = CGame::GetInstance()->GetPlayerData();

		if (playerData->GetHearts() > 0)
		{
			if (playerData->GetSubWeaponType() == "stopwatch")
			{
				playerData->DecreaseHearts(2);
			}
			else
			{
				playerData->DecreaseHearts(1);
			}
		}
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

		if (delayEndTime == -1)
		{
			delayEndTime = GetTickCount() + settingManager->GetIntValue("SIMON_DELAY_TIME");
		}

		break;

	case SIMON_STATE_IDLE:
		if (!onBar)
		{
			vx = 0;
		}

		if (deflecting)
		{
			SetState(SIMON_STATE_SIT);
			sittingCounter++;
		}

		touchingGround = true;
		sitting = false;
		ResetAnimations();
		break;

	case SIMON_STATE_CUT_SCENE_AUTO_WALK:
		if (movingDirection == "rightward")
		{
			directionX = Direction::Right;
			vx = settingManager->GetFloatValue("SIMON_SLOW_WALK_SPEED");
		}
		else if (movingDirection == "leftward")
		{
			directionX = Direction::Left;
			vx = -settingManager->GetFloatValue("SIMON_SLOW_WALK_SPEED");
		}

		break;

	case SIMON_STATE_IDLE_ON_STAIR:
		vx = 0;
		vy = 0;

		break;

	case SIMON_STATE_WALK_DOWNSTAIR:
		if (stairDirectionX == directionX)
		{
			vx = stairDirectionX == Direction::Right ? settingManager->GetFloatValue("SIMON_WALK_SPEED") : -settingManager->GetFloatValue("SIMON_WALK_SPEED");
		}
		else
		{
			vx = stairDirectionX == Direction::Right ? -settingManager->GetFloatValue("SIMON_WALK_SPEED") : settingManager->GetFloatValue("SIMON_WALK_SPEED");
		}

		vy = settingManager->GetFloatValue("SIMON_WALK_SPEED");
		directionY = Direction::Down;
		break;

	case SIMON_STATE_WALK_UPSTAIR:
		if (stairDirectionX == directionX)
		{
			vx = stairDirectionX == Direction::Right ? settingManager->GetFloatValue("SIMON_WALK_SPEED") : -settingManager->GetFloatValue("SIMON_WALK_SPEED");
		}
		else
		{
			vx = stairDirectionX == Direction::Right ? -settingManager->GetFloatValue("SIMON_WALK_SPEED") : settingManager->GetFloatValue("SIMON_WALK_SPEED");
		}

		vy = -settingManager->GetFloatValue("SIMON_WALK_SPEED");
		directionY = Direction::Up;
		break;

	case SIMON_STATE_DEFLECT:
		CGame::GetInstance()->GetSoundManager()->Play("deflecting");

		if (directionX == Direction::Right)
		{
			vy = -settingManager->GetFloatValue("SIMON_DEFLECT_SPEED_Y");
			vx = -settingManager->GetFloatValue("SIMON_DEFLECT_SPEED_X");
		}
		else
		{
			vy = -settingManager->GetFloatValue("SIMON_DEFLECT_SPEED_Y");
			vx = settingManager->GetFloatValue("SIMON_DEFLECT_SPEED_X");
		}

		animationSet->at(GetAnimationToRender())->SetStartTime(GetTickCount());

		break;

	case SIMON_STATE_WATCH:
		vx = 0;
		switchSceneEnabled = true;

		if (switchSceneTime == -1)
		{
			switchSceneTime = GetTickCount() + waitingTime;
		}

		break;

	case SIMON_STATE_DIE:
		vx = 0;

		CGame::GetInstance()->GetSoundManager()->StopBackgroundSounds();
		CGame::GetInstance()->GetSoundManager()->Play("dying");

		switchSceneEnabled = true;

		if (switchSceneTime == -1)
		{
			switchSceneTime = GetTickCount() + waitingTime;
		}

		break;
	}
}

void CSimon::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

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

	if (whip)
	{
		whip->ResetAnimations();
	}
}

void CSimon::Pause()
{
	for (int i = 0; i < animationSet->size(); i++)
	{
		animationSet->at(i)->Pause();
	}

	if (whip)
	{
		whip->Pause();
	}
}

void CSimon::HandleGravity()
{
	if (!onStair && state != SIMON_STATE_WALK_UPSTAIR && state != SIMON_STATE_CUT_SCENE_AUTO_WALK && state != SIMON_STATE_WATCH)
	{
		vy += CSettingManager::GetInstance()->GetFloatValue("SIMON_GRAVITY") * dt;
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

			CGame::GetInstance()->GetSoundManager()->Play("using_whip");
			whip->Update(dt, coObjects);
		}
	}
}

void CSimon::HandleAttackWithSubWeapon(vector<LPGAMEOBJECT>* coObjects)
{
	if (state == SIMON_STATE_STAND_AND_THROW || state == SIMON_STATE_STAND_ON_STAIR_AND_THROW)
	{
		if (lastFrameShown == false && AbleToThrowSubWeapon() && animationSet->at(GetAnimationToRender())->ReachLastFrame())
		{
			lastFrameShown = true;

			string type = CGame::GetInstance()->GetPlayerData()->GetSubWeaponType();
			CGameObject* weapon = nullptr;

			if (type == "dagger")
			{
				CGame::GetInstance()->GetSoundManager()->Play("throwing_dagger");
				weapon = new WDagger();
			}
			else if (type == "boomerang")
			{
				CGame::GetInstance()->GetSoundManager()->Play("throwing_boomerang");
				weapon = new WBoomerang(this);
			}
			else if (type == "axe")
			{
				CGame::GetInstance()->GetSoundManager()->Play("throwing_axe");
				weapon = new WAxe();
			}
			else if (type == "holy_water")
			{
				weapon = new CHolyWaterBottle();
			}
			else if (type == "stopwatch")
			{
				weapon = new WStopwatch();
			}

			weapon->SetDirectionX(directionX);
			weapon->SetPosition(directionX == Direction::Right ? x + 40.0f : x - 20.0f, y + 12.0f);

			CGame::GetInstance()->GetPlayerData()->AddThrownSubWeapons();

			CGrid* grid = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->GetGrid();
			CUnit* unit = new CUnit(grid, weapon, weapon->x, weapon->y);
		}
	}
}

void CSimon::HandleCollisionObjects(vector<LPGAMEOBJECT>* coObjects)
{
	if (sittingCounter != 0 && sittingCounter % 20 == 0)
	{
		deflecting = false;
		sittingCounter = 0;

		SetState(SIMON_STATE_IDLE);
	}

	if (coObjects && state != SIMON_STATE_DIE)
	{
		int countAtBottom = 0, countAtTop = 0, countAtBi = 0;
		CGame* game = CGame::GetInstance();
		CGameSoundManager* soundManager = game->GetSoundManager();
		CSettingManager* settingManager = CSettingManager::GetInstance();

		for (int i = 0; i < coObjects->size(); i++)
		{
			auto object = coObjects->at(i);

			if (game->HaveCollision(this, object))
			{
				if (object->isItem)
				{
					if (dynamic_cast<CRosary*>(object))
					{
						soundManager->Play("collecting_rosary");

						for (int j = 0; j < coObjects->size(); j++)
						{
							if (dynamic_cast<CEnemy*>(coObjects->at(j)))
							{
								coObjects->at(j)->Disappear();
							}
						}

						object->removable = true;
					}
					else
					{
						HandleCollisionWithItems(object);
					}
				}
				else if (dynamic_cast<CEnemy*>(object))
				{
					HandleCollisionWithEnemies(object);
				}
				else if (dynamic_cast<CEasterEgg*>(object))
				{
					auto easterEgg = dynamic_cast<CEasterEgg*>(object);

					if (!easterEgg->MustSit() || (easterEgg->MustSit() && sitting == true))
					{
						easterEgg->ShowHiddenItem();
						easterEgg->removable = true;
					}
				}
				else if (dynamic_cast<CTopStair*>(object))
				{
					auto stair = dynamic_cast<CTopStair*>(object);

					float stairLeft, stairTop, stairRight, stairBottom, stairCenterX;
					stair->GetBoundingBox(stairLeft, stairTop, stairRight, stairBottom);

					float simonLeft, simonTop, simonRight, simonBottom, simonCenterX;
					GetBoundingBox(simonLeft, simonTop, simonRight, simonBottom);

					stairCenterX = stairLeft + (stairRight - stairLeft) / 2;
					simonCenterX = simonLeft + (simonRight - simonLeft) / 2;

					bool validX = false;
					bool validY = false;
					bool canTurnBack = false;

					validY = simonTop > stairTop ? true : false;

					if (directionX == stair->directionX)
					{
						if (directionX == Direction::Right)
						{
							validX = simonCenterX >= stairRight && simonCenterX <= stairRight + 10 ? true : false;
						}
						else
						{
							validX = simonCenterX <= stairLeft && simonCenterX > stairLeft - 10 ? true : false;
						}
					}
					else
					{
						validX = false;
					}

					if (stair->directionX == Direction::Right)
					{
						canTurnBack = simonCenterX >= stairRight && simonCenterX <= stairRight + 10 ? true : false;
					}
					else
					{
						canTurnBack = simonCenterX <= stairLeft && simonCenterX > stairLeft - 10 ? true : false;
					}

					if (validX && validY)
					{
						countAtTop++;
					}

					if (!onStair)
					{
						if (down && canTurnBack)
						{
							directionX = stair->directionX;
							onStair = true;
						}

						directionY = stair->directionY;
						stairDirectionX = stair->directionX;
						stairDirectionY = stair->directionY;
					}
					else
					{
						bool reversedDirectionX = (stair->directionX == Direction::Right && directionX == Direction::Left) || (stair->directionX == Direction::Left && directionX == Direction::Right);
						bool reversedDirectionY = stair->directionY == Direction::Down && directionY == Direction::Up;

						if (reversedDirectionX && reversedDirectionY)
						{
							onStair = false;
							SetState(SIMON_STATE_IDLE);
						}
					}
				}
				else if (dynamic_cast<CBottomStair*>(object))
				{
					auto stair = dynamic_cast<CBottomStair*>(object);

					float stairLeft, stairTop, stairRight, stairBottom, stairCenterX;
					stair->GetBoundingBox(stairLeft, stairTop, stairRight, stairBottom);

					float simonLeft, simonTop, simonRight, simonBottom, simonCenterX;
					GetBoundingBox(simonLeft, simonTop, simonRight, simonBottom);

					stairCenterX = stairLeft + (stairRight - stairLeft) / 2;
					simonCenterX = simonLeft + (simonRight - simonLeft) / 2;

					bool validX = false;
					bool validY = false;
					bool canTurnBack = false;

					validY = simonTop < stairTop ? true : false;

					if (directionX == stair->directionX)
					{
						if (directionX == Direction::Right)
						{
							validX = simonCenterX <= stairLeft && simonCenterX > stairLeft - 10 ? true : false;
						}
						else
						{
							validX = simonCenterX >= stairRight && simonCenterX <= stairRight + 10 ? true : false;
						}
					}
					else
					{
						validX = false;
					}

					if (stair->directionX == Direction::Right)
					{
						canTurnBack = simonCenterX <= stairLeft && simonCenterX > stairLeft - 10 ? true : false;
					}
					else
					{
						canTurnBack = simonCenterX >= stairRight && simonCenterX <= stairRight + 10 ? true : false;
					}

					if (validX && validY)
					{
						countAtBottom++;
					}

					if (!onStair)
					{
						if (up && canTurnBack)
						{
							directionX = stair->directionX;
							onStair = true;
						}

						directionY = stair->directionY;
						stairDirectionX = stair->directionX;
						stairDirectionY = stair->directionY;
					}
					else
					{
						bool reversedDirectionX = (stair->directionX == Direction::Right && directionX == Direction::Left) || (stair->directionX == Direction::Left && directionX == Direction::Right);
						bool reversedDirectionY = stair->directionY == Direction::Up && directionY == Direction::Down;

						if (reversedDirectionX && reversedDirectionY)
						{
							onStair = false;
							SetState(SIMON_STATE_IDLE);
						}
					}
				}
				else if (dynamic_cast<CBiStair*>(object))
				{
					auto stair = dynamic_cast<CBiStair*>(object);

					float stairLeft, stairTop, stairRight, stairBottom, stairCenterX;
					stair->GetBoundingBox(stairLeft, stairTop, stairRight, stairBottom);

					float simonLeft, simonTop, simonRight, simonBottom, simonCenterX;
					GetBoundingBox(simonLeft, simonTop, simonRight, simonBottom);

					stairCenterX = stairLeft + (stairRight - stairLeft) / 2;
					simonCenterX = simonLeft + (simonRight - simonLeft) / 2;

					bool validX = false;
					bool validY = false;
					bool canTurnBack = false;

					validY = simonTop < stairTop ? true : false;

					upsideDirectionX = stair->upsideDirectionX;
					upsideDirectionY = stair->upsideDirectionY;
					downsideDirectionX = stair->downsideDirectionX;
					downsideDirectionY = stair->downsideDirectionY;

					if (!onStair)
					{
						if (up)
						{
							if (stair->upsideDirectionX == Direction::Right)
							{
								validX = canTurnBack = simonCenterX >= stairRight + 10 && simonCenterX <= stairRight + 20 ? true : false;
							}
							else
							{
								validX = canTurnBack = simonCenterX <= stairLeft - 10 && simonCenterX > stairLeft - 20 ? true : false;
							}

							if (validX && validY)
							{
								countAtBi++;
							}
							else
							{
								return;
							}

							if (canTurnBack)
							{
								directionX = stair->directionX;
							}

							directionY = stair->upsideDirectionY;
							stairDirectionX = stair->upsideDirectionX;
							stairDirectionY = stair->upsideDirectionY;

							if (directionX == stairDirectionX)
							{
								onStair = true;
								SetState(SIMON_STATE_WALK_UPSTAIR);
							}
						}
						
						if (down)
						{
							if (stair->downsideDirectionX == Direction::Right)
							{
								validX = canTurnBack = simonCenterX >= stairRight + 10 && simonCenterX <= stairRight + 20 ? true : false;
							}
							else
							{
								validX = canTurnBack = simonCenterX <= stairLeft - 10 && simonCenterX > stairLeft - 20 ? true : false;
							}

							if (validX && validY)
							{
								countAtBi++;
							}
							else
							{
								return;
							}

							if (canTurnBack)
							{
								directionX = stair->directionX;
							}

							directionY = stair->downsideDirectionY;
							stairDirectionX = stair->downsideDirectionX;
							stairDirectionY = stair->downsideDirectionY;

							if (directionX == stairDirectionX)
							{
								onStair = true;
								SetState(SIMON_STATE_WALK_DOWNSTAIR);
							}
						}
					}
					else
					{
						bool reversedDirectionX = (downsideDirectionX == Direction::Right && directionX == Direction::Left) || (upsideDirectionX == Direction::Right && directionX == Direction::Left);
						bool reversedDirectionY = (downsideDirectionY == Direction::Down && directionY == Direction::Up) || (upsideDirectionY == Direction::Up && directionY == Direction::Down);
						bool reachedTop = directionY == Direction::Up && y + SIMON_BBOX_HEIGHT <= stair->y + BI_STAIR_BBOX_HEIGHT;
						bool reachedBottom = directionY == Direction::Down && y + SIMON_BBOX_HEIGHT >= stair->y + BI_STAIR_BBOX_HEIGHT;

						if (reversedDirectionX && reversedDirectionY && (reachedTop || reachedBottom))
						{
							onStair = false;
							SetState(SIMON_STATE_IDLE);
						}
					}
				}
				else if (dynamic_cast<CEnemySpawnerArea*>(object))
				{
					auto area = dynamic_cast<CEnemySpawnerArea*>(object);

					area->SpawnEnemy();
				}
				else if (dynamic_cast<WBoomerang*>(object))
				{
					auto boomerang = dynamic_cast<WBoomerang*>(object);

					if (boomerang->GetCollisionCount() >= 1)
					{
						CGame::GetInstance()->GetSoundManager()->Stop("throwing_boomerang");
						CGame::GetInstance()->GetPlayerData()->DecreaseThrownSubWeapons();

						object->removable = true;
					}
				}
				else if (dynamic_cast<WFireball*>(object) || dynamic_cast<WBone*>(object))
				{
					if (!partiallyInvisible && !fullyInvisible)
					{
						partiallyInvisible = true;

						if (invisibleTimeout == -1)
						{
							invisibleTimeout = GetTickCount() + settingManager->GetIntValue("SIMON_INVISIBILITY_TIME");
						}

						if (!onStair)
						{
							deflecting = true;
							SetState(SIMON_STATE_DEFLECT);
						}

						CGame::GetInstance()->GetPlayerData()->DecreaseHealthVolumes(dynamic_cast<CEnemy*>(object)->GetDamages());

						object->removable = true;
					}
				}
			}
			else if (dynamic_cast<CEnemy*>(object))
			{
				auto enemy = dynamic_cast<CEnemy*>(object);

				if (CEnemy::IsPlayerNearby(enemy->x, enemy->y, x, y, enemy->GetAreaRadiusX(), enemy->GetAreaRadiusY()))
				{
					enemy->OnPlayerEnterArea();
				}
			}
		}

		atTopStair = touchingGround && countAtTop > 0;
		atBottomStair = touchingGround && countAtBottom > 0;
		atBiStair = touchingGround && countAtBi > 0;
	}
}

void CSimon::HandleCollisionWithItems(CGameObject* item)
{
	CPlayerData* playerData = CGame::GetInstance()->GetPlayerData();
	CGameSoundManager* soundManager = CGame::GetInstance()->GetSoundManager();
	CSettingManager* settingManager = CSettingManager::GetInstance();

	if (dynamic_cast<CMorningStar*>(item))
	{
		soundManager->Play("collecting_weapon_item");
		SetState(SIMON_STATE_DELAY);
		playerData->SetWhipLevel(playerData->GetWhipLevel() + 1);
	}
	else if (dynamic_cast<CBigHeart*>(item))
	{
		soundManager->Play("collecting_item");
		playerData->AddHearts(settingManager->GetIntValue("BIG_HEART_HEARTS"));
	}
	else if (dynamic_cast<CSmallHeart*>(item))
	{
		soundManager->Play("collecting_item");
		playerData->AddHearts(settingManager->GetIntValue("SMALL_HEART_HEARTS"));
	}
	else if (dynamic_cast<CDagger*>(item))
	{
		soundManager->Play("collecting_weapon_item");
		playerData->SetSubWeaponType("dagger");
	}
	else if (dynamic_cast<CAxe*>(item))
	{
		soundManager->Play("collecting_weapon_item");
		playerData->SetSubWeaponType("axe");
	}
	else if (dynamic_cast<CMoneyBag*>(item))
	{
		soundManager->Play("collecting_item");
		playerData->AddScore(settingManager->GetIntValue("MONEY_BAG_SCORE"));
	}
	else if (dynamic_cast<CRedMoneyBag*>(item))
	{
		soundManager->Play("collecting_item");
		playerData->AddScore(settingManager->GetIntValue("RED_MONEY_BAG_SCORE"));
	}
	else if (dynamic_cast<CPurpleMoneyBag*>(item))
	{
		soundManager->Play("collecting_item");
		playerData->AddScore(settingManager->GetIntValue("PURPLE_MONEY_BAG_SCORE"));
	}
	else if (dynamic_cast<CWhiteMoneyBag*>(item))
	{
		soundManager->Play("collecting_item");
		playerData->AddScore(settingManager->GetIntValue("WHITE_MONEY_BAG_SCORE"));
	}
	else if (dynamic_cast<CCrown*>(item))
	{
		soundManager->Play("collecting_item");
		playerData->AddScore(settingManager->GetIntValue("CROWN_SCORE"));
	}
	else if (dynamic_cast<CBoomerang*>(item))
	{
		soundManager->Play("collecting_weapon_item");
		playerData->SetSubWeaponType("boomerang");
	}
	else if (dynamic_cast<CDoubleShot*>(item))
	{
		soundManager->Play("collecting_weapon_item");
		playerData->SetPower(DOUBLE_POWER);
	}
	else if (dynamic_cast<CTripleShot*>(item))
	{
		soundManager->Play("collecting_weapon_item");
		playerData->SetPower(TRIPLE_POWER);
	}
	else if (dynamic_cast<CPorkChop*>(item))
	{
		soundManager->Play("collecting_item");
		playerData->AddHealthVolumes(settingManager->GetIntValue("PORK_CHOP_HEALTHS"));
	}
	else if (dynamic_cast<CHolyWater*>(item))
	{
		soundManager->Play("collecting_weapon_item");
		playerData->SetSubWeaponType("holy_water");
	}
	else if (dynamic_cast<CStopwatch*>(item))
	{
		soundManager->Play("collecting_weapon_item");
		playerData->SetSubWeaponType("stopwatch");
	}
	else if (dynamic_cast<CInvisibilityPotion*>(item))
	{
		soundManager->Play("collecting_weapon_item");

		fullyInvisible = true;

		if (invisibleTimeout == -1)
		{
			soundManager->Play("starting_invisibility");
			invisibleTimeout = GetTickCount() + settingManager->GetIntValue("SIMON_INVISIBILITY_TIME");
		}
	}
	else if (dynamic_cast<CMagicCrystal*>(item))
	{
		soundManager->StopBackgroundSounds();
		soundManager->Play("clearing_all_stages");

		startingEndingCounter = true;
	}

	item->Disappear();
}

void CSimon::HandleCollisionWithEnemies(CGameObject* item)
{
	CPlayerData* playerData = CGame::GetInstance()->GetPlayerData();
	CSettingManager* settingManager = CSettingManager::GetInstance();

	if (!partiallyInvisible && !fullyInvisible)
	{
		bool deflectable = false;
		bool invisible = false;

		if (dynamic_cast<CWhiteSkeleton*>(item))
		{
			auto whiteSkeleton = dynamic_cast<CWhiteSkeleton*>(item);

			if (whiteSkeleton->GetState() != WHITE_SKELETON_STATE_DIE)
			{
				invisible = true;

				playerData->DecreaseHealthVolumes(dynamic_cast<CEnemy*>(item)->GetDamages());
			}
		}
		else
		{
			invisible = true;

			if (dynamic_cast<CRaven*>(item))
			{
				item->Disappear();
				playerData->DecreaseHealthVolumes(dynamic_cast<CEnemy*>(item)->GetDamages());
			}
			else if (dynamic_cast<CRedBat*>(item))
			{
				item->HideHiddenItem();
				item->Disappear();

				playerData->DecreaseHealthVolumes(dynamic_cast<CEnemy*>(item)->GetDamages());
			}
			else
			{
				playerData->DecreaseHealthVolumes(dynamic_cast<CEnemy*>(item)->GetDamages());
			}
		}

		if (!onStair)
		{
			if (dynamic_cast<CWhiteSkeleton*>(item))
			{
				auto whiteSkeleton = dynamic_cast<CWhiteSkeleton*>(item);

				if (whiteSkeleton->GetState() != WHITE_SKELETON_STATE_DIE)
				{
					deflectable = true;
				}
			}
			else
			{
				deflectable = true;
			}
		}

		if (invisible)
		{
			partiallyInvisible = true;

			if (invisibleTimeout == -1)
			{
				invisibleTimeout = GetTickCount() + settingManager->GetIntValue("SIMON_INVISIBILITY_TIME");
			}
		}

		if (deflectable)
		{
			deflecting = true;
			jumpable = false;
			SetState(SIMON_STATE_DEFLECT);
		}
	}
}

void CSimon::HandleSwitchScene()
{
	CGame* game = CGame::GetInstance();
	CSettingManager* settingManager = CSettingManager::GetInstance();

	if (state == SIMON_STATE_CUT_SCENE_AUTO_WALK)
	{
		if (switchScenePosition == "right")
		{
			if (x >= settingManager->GetIntValue("SCREEN_WIDTH"))
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

					switchSceneEnabled = true;
				}
			}
		}
		else if (switchScenePosition == "center")
		{
			if (x <= (settingManager->GetIntValue("SCREEN_WIDTH") / 2) - 50)
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

					switchSceneEnabled = true;
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

					switchSceneEnabled = true;
				}
			}
		}
	}

	if (switchSceneEnabled && switchSceneTime != -1 && GetTickCount() > switchSceneTime)
	{
		switchSceneTime = -1;

		if (state != SIMON_STATE_DIE)
		{
			game->GetSceneManager()->SwitchScene(game->GetSceneManager()->GetNextSceneId());
		}
		else
		{
			if (game->GetPlayerData()->GetLives() > 0)
			{
				game->GetPlayerData()->DecreaseLives();
				game->GetSceneManager()->SwitchSceneByIndex(game->GetSceneManager()->GetCurrentSceneIndex());

				game->GetPlayerData()->ResetHealthVolumes();
				game->GetPlayerData()->ResetPower();
				game->GetPlayerData()->ResetWeapons();

				game->GetBossData()->Reset();
			}
			else
			{
				game->GetTimer()->Pause();
				game->GetSoundManager()->StopBackgroundSounds();
				game->GetSoundManager()->Play("game_over_background_music");

				dynamic_cast<CPlayScene*>(game->GetSceneManager()->GetCurrentScene())->ShowGameOverBoard();
				game->GetCamera()->SetPosition(0, 0);
			}
		}
	}
}

void CSimon::HandleInvisibility()
{
	if ((partiallyInvisible || fullyInvisible) && invisibleTimeout != -1 && GetTickCount() >= invisibleTimeout)
	{
		if (fullyInvisible)
		{
			CGame::GetInstance()->GetSoundManager()->Play("ending_invisibility");
		}

		partiallyInvisible = false;
		fullyInvisible = false;
		invisibleTimeout = -1;
	}
}

void CSimon::HandleEndingGame()
{
	if (startingEndingCounter)
	{
		endingCounter += 1;

		if (endingCounter % 250 == 0)
		{
			CGame::GetInstance()->End();
		}
	}
}

void CSimon::HandleJumpingOutOfWorld()
{
	if (state != SIMON_STATE_DIE && y > CSettingManager::GetInstance()->GetIntValue("SCREEN_HEIGHT"))
	{
		SetState(SIMON_STATE_DIE);
	}
}

void CSimon::RenderWhip()
{
	if (whip && (state == SIMON_STATE_SIT_AND_ATTACK || state == SIMON_STATE_STAND_AND_ATTACK || state == SIMON_STATE_WALK_DOWNSTAIR_AND_ATTACK || state == SIMON_STATE_WALK_UPSTAIR_AND_ATTACK))
	{
		whip->Render();
	}
}

bool CSimon::AbleToThrowSubWeapon()
{
	CPlayerData* playerData = CGame::GetInstance()->GetPlayerData();

	if (playerData->GetPower() == NORMAL_POWER)
	{
		if (playerData->GetThrownSubWeapons() == 0)
		{
			return true;
		}
	}
	else if (playerData->GetPower() == DOUBLE_POWER)
	{
		if (playerData->GetThrownSubWeapons() < DOUBLE_POWER)
		{
			return true;
		}
	}
	else
	{
		if (playerData->GetThrownSubWeapons() < TRIPLE_POWER)
		{
			return true;
		}
	}

	return false;
}

CSimon::~CSimon()
{
	SAFE_DELETE(whip);
	SAFE_DELETE(subWeapon);
}