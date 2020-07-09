#include "Castle.h"

CCastle::CCastle()
{
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

	SetAnimationSet("castle");
	animationSet->at(0)->SetStartTime(GetTickCount());

	movingCounter = 0;
	vx = settingManager->GetFloatValue("CASTLE_MOVE_SPEED_X");
	vy = settingManager->GetFloatValue("CASTLE_MOVE_SPEED_Y");
}

void CCastle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	movingCounter += 1;

	if (movingCounter % 2)
	{
		vx = -vx;
	}

	if (y > CGame::GetInstance()->GetSettingManager()->GetIntValue("SCREEN_HEIGHT"))
	{
		removable = true;
	}

	x += vx * dt;
	y += vy * dt;
}

void CCastle::Render()
{
	animationSet->at(0)->Render(x, y);
}

void CCastle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}