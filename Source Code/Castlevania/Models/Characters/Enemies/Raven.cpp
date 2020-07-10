#include "Raven.h"
#include "../../../Game.h"
#include "../../Misc/BottomStair.h"
#include "../../Weapons/HolyWaterBottle.h"

CRaven::CRaven(CSimon* simon)
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	this->mustInArea = true;
	this->simon = simon;
	this->areaRadiusX = settingManager->GetIntValue("RAVEN_AREA_RADIUS_X");
	this->areaRadiusY = settingManager->GetIntValue("RAVEN_AREA_RADIUS_Y");
	this->flyingCounter = 0;

	SetAnimationSet("raven");
	SetState(settingManager->GetIntValue("RAVEN_INITIAL_STATE"));
}

void CRaven::SetState(int state)
{
	CGameObject::SetState(state);
	CSettingManager* settingManager = CSettingManager::GetInstance();

	switch (state)
	{
	case RAVEN_STATE_IDLE:
		break;

	case RAVEN_STATE_FLY:
		GenerateTarget();
		break;

	case RAVEN_STATE_ATTACK:
		if (x <= simon->x)
		{
			targetX = simon->x + settingManager->GetIntValue("SIMON_BBOX_WIDTH");
		}
		else
		{
			targetX = simon->x;
		}

		targetY = simon->y + 10;
		break;
	}
}

void CRaven::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);
	CSettingManager* settingManager = CSettingManager::GetInstance();

	bool softPaused = CGame::GetInstance()->GetSceneManager()->GetCurrentScene()->SoftPaused();

	if (softPaused) return;

	angle = atan2(targetY - y, targetX - x);

	if (state == RAVEN_STATE_FLY)
	{
		if (ReachedTarget())
		{
			directionX = x <= simon->x ? Direction::Right : Direction::Left;

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

			vx = settingManager->GetFloatValue("RAVEN_ATTACK_SPEED_X");
			vy = settingManager->GetFloatValue("RAVEN_ATTACK_SPEED_Y");

			x += vx * cos(angle) * dt;
			y += vy * sin(angle) * dt;
		}
	}

	if (state == RAVEN_STATE_ATTACK)
	{
		if (ReachedTarget())
		{
			SetState(RAVEN_STATE_FLY);
		}
		else
		{
			directionX = x <= targetX ? Direction::Right : Direction::Left;

			vx = settingManager->GetFloatValue("RAVEN_ATTACK_SPEED_X");
			vy = settingManager->GetFloatValue("RAVEN_ATTACK_SPEED_Y");

			x += vx * cos(angle) * dt;
			y += vy * sin(angle) * dt;
		}
	}
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
		if (CGame::GetInstance()->BoundingBoxDisplayed())
		{
			RenderBoundingBox();
		}

		animationSet->at(ani)->Render(x, y);
	}
}

void CRaven::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	if (!showingEndingEffect)
	{
		CSettingManager* settingManager = CSettingManager::GetInstance();

		l = x;
		t = y;
		r = l + settingManager->GetIntValue("RAVEN_BBOX_WIDTH");
		b = t + settingManager->GetIntValue("RAVEN_BBOX_HEIGHT");
	}
}

void CRaven::TakeDamage(int damages)
{
	CEnemy::TakeDamage(damages);

	if (attacks <= 0)
	{
		Disappear();
		CGame::GetInstance()->GetPlayerData()->AddScore(CSettingManager::GetInstance()->GetIntValue("RAVEN_SCORE"));
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
	CEnemy::OnPlayerEnterArea();

	if (state == RAVEN_STATE_IDLE)
	{
		SetState(RAVEN_STATE_ATTACK);
	}
}

int CRaven::GetDamages()
{
	return CSettingManager::GetInstance()->GetIntValue("RAVEN_DAMAGES");
}

bool CRaven::ReachedTarget()
{
	float distance = sqrt(pow(x - targetX, 2) + pow(y - targetY, 2));

	if (distance <= 10)
	{
		return true;
	}

	return false;
}

void CRaven::GenerateTarget()
{
	float cameraLeft = CGame::GetInstance()->GetCamera()->GetLeft();
	float cameraRight = CGame::GetInstance()->GetCamera()->GetRight();

	int boundingBoxWidth = CSettingManager::GetInstance()->GetIntValue("RAVEN_BBOX_WIDTH");

	if (y >= simon->y - 50)
	{
		targetX = directionX == Direction::Right ? x + 160 : x - 160;
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
		targetX = x <= targetX ? x + 160 : x - 160;
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
