#include "PhantomBat.h"
#include "../../../Game.h"
#include "../../Weapons/WAxe.h"

CPhantomBat::CPhantomBat(CSimon* simon)
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	this->mustInArea = true;
	this->simon = simon;
	this->areaRadiusX = settingManager->GetIntValue("PHANTOM_BAT_AREA_RADIUS_X");
	this->areaRadiusY = settingManager->GetIntValue("PHANTOM_BAT_AREA_RADIUS_Y");
	this->flyingCounter = 0;
	this->attacks = settingManager->GetIntValue("PHANTOM_BAT_ATTACKS");

	SetAnimationSet("phantom_bat");
	SetState(settingManager->GetIntValue("PHANTOM_BAT_INITIAL_STATE"));
}

void CPhantomBat::SetState(int state)
{
	CGameObject::SetState(state);
	CSettingManager* settingManager = CSettingManager::GetInstance();

	switch (state)
	{
	case PHANTOM_BAT_STATE_IDLE:
		break;

	case PHANTOM_BAT_STATE_FLY:
		GenerateTarget();
		break;

	case PHANTOM_BAT_STATE_ATTACK:
		if (x <= simon->x)
		{
			targetX = simon->x + SIMON_BBOX_WIDTH;
		}
		else
		{
			targetX = simon->x;
		}

		targetY = simon->y + 10;
		break;

	case PHANTOM_BAT_STATE_DELAY:
		delayTimeout = GetTickCount() + settingManager->GetIntValue("ENEMY_DELAY_TIME");
		break;

	case PHANTOM_BAT_STATE_DIE:
		x = lastPosX;
		y = lastPosY;
		break;
	}
}

void CPhantomBat::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CSettingManager* settingManager = CSettingManager::GetInstance();

	bool softPaused = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->SoftPaused();

	if (softPaused) return;

	if (state != PHANTOM_BAT_STATE_DIE)
	{
		angle = atan2(targetY - y, targetX - x);

		if (state == PHANTOM_BAT_STATE_DELAY && delayTimeout != -1 && GetTickCount() > delayTimeout)
		{
			if (GetTickCount() > delayTimeout + settingManager->GetIntValue("ENEMY_DELAY_PLUS_TIME"))
			{
				delayTimeout = -1;
			}

			Attack();
		}

		if (state == PHANTOM_BAT_STATE_FLY)
		{
			if (ReachedTarget())
			{
				flyingCounter += 1;

				if (flyingCounter % 30 == 0)
				{
					Attack();
					flyingCounter = 0;
				}
			}
			else
			{
				directionX = x <= targetX ? Direction::Right : Direction::Left;

				vx = settingManager->GetFloatValue("PHANTOM_BAT_ATTACK_SPEED_X");
				vy = settingManager->GetFloatValue("PHANTOM_BAT_ATTACK_SPEED_Y");

				x += vx * cos(angle) * dt;
				y += vy * sin(angle) * dt;
			}
		}

		if (state == PHANTOM_BAT_STATE_ATTACK)
		{
			if (ReachedTarget())
			{
				directionX = directionX == Direction::Left ? Direction::Right : Direction::Left;

				SetState(PHANTOM_BAT_STATE_FLY);
			}
			else
			{
				directionX = x <= targetX ? Direction::Right : Direction::Left;

				vx = settingManager->GetFloatValue("PHANTOM_BAT_ATTACK_SPEED_X");
				vy = settingManager->GetFloatValue("PHANTOM_BAT_ATTACK_SPEED_Y");

				x += vx * cos(angle) * dt;
				y += vy * sin(angle) * dt;
			}
		}
	}
}

void CPhantomBat::Render()
{
	int ani;

	switch (state)
	{
	case PHANTOM_BAT_STATE_IDLE:
		ani = PHANTOM_BAT_ANI_IDLE;
		break;

	case PHANTOM_BAT_STATE_ATTACK:
	case PHANTOM_BAT_STATE_FLY:
		ani = PHANTOM_BAT_ANI_FLY;
		break;

	case PHANTOM_BAT_STATE_DELAY:
		ani = PHANTOM_BAT_ANI_DELAY;
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

void CPhantomBat::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (!showingEndingEffect)
	{
		CSettingManager* settingManager = CSettingManager::GetInstance();

		l = x;
		t = y;
		r = l + PHANTOM_BAT_BBOX_WIDTH;
		b = t + PHANTOM_BAT_BBOX_HEIGHT;
	}
}

void CPhantomBat::TakeDamage(int damages)
{
	CEnemy::TakeDamage(damages);

	CGame::GetInstance()->GetBossData()->DecreaseHealthVolumes(damages);

	if (attacks <= 0)
	{
		lastPosX = x;
		lastPosY = y;

		SetState(PHANTOM_BAT_STATE_DIE);

		Disappear();
		CGame::GetInstance()->GetPlayerData()->AddScore(CSettingManager::GetInstance()->GetIntValue("PHANTOM_BAT_SCORE"));
	}
	else
	{
		ShowEffect();
		SetState(PHANTOM_BAT_STATE_DELAY);
	}
}

void CPhantomBat::Attack()
{
	SetState(PHANTOM_BAT_STATE_ATTACK);
}

void CPhantomBat::OnPlayerEnterArea()
{
	CEnemy::OnPlayerEnterArea();

	if (CGame::GetInstance()->GetCamera()->Locked())
	{
		if (state == PHANTOM_BAT_STATE_IDLE)
		{
			SetState(PHANTOM_BAT_STATE_FLY);
		}
	}
}

int CPhantomBat::GetDamages()
{
	return CSettingManager::GetInstance()->GetIntValue("BOSS_DEFAULT_DAMAGES");
}

bool CPhantomBat::ReachedTarget()
{
	float distance = sqrt(pow(x - targetX, 2) + pow(y - targetY, 2));

	if (distance <= 10)
	{
		return true;
	}

	return false;
}

void CPhantomBat::GenerateTarget()
{
	float cameraLeft = CGame::GetInstance()->GetCamera()->GetLeft();
	float cameraRight = CGame::GetInstance()->GetCamera()->GetRight();

	int boundingBoxWidth = PHANTOM_BAT_BBOX_WIDTH;

	if (y >= simon->y - 50)
	{
		targetX = directionX == Direction::Right ? x - 160 : x + 160;
		targetY = y - 140;

		if (targetX < cameraLeft + 20)
		{
			targetX = cameraLeft + 20;
		}

		if (targetX > cameraRight - boundingBoxWidth - 20)
		{
			targetX = cameraRight - boundingBoxWidth - 20;
		}
	}
	else
	{
		targetX = x + 160;
		targetY = y + 50;

		if (targetX < cameraLeft + 20)
		{
			targetX = cameraLeft + 20;
		}

		if (targetX > cameraRight - boundingBoxWidth - 20)
		{
			targetX = cameraRight - boundingBoxWidth - 20;
		}
	}
}
