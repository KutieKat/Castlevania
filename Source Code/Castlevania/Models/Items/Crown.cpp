#include "Crown.h"
#include "../../Game.h"

CCrown::CCrown()
{
	isItem = true;
	SetAnimationSet("crown");
}

void CCrown::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt);

	y += vy * dt;

	if (y < topBound)
	{
		vy = 0;
	}
}

void CCrown::SetState(int state)
{
	CGameObject::SetState(state);
	CSettingManager* settingManager = CSettingManager::GetInstance();

	switch (state)
	{
	case ITEM_STATE_APPEAR:
		CGame::GetInstance()->GetSoundManager()->Play("showing_bonus");
		vy = -settingManager->GetFloatValue("BONUS_MOVE_UP_SPEED");
		break;
	}
}

void CCrown::Render()
{
	if (!showingEndingEffect)
	{
		if (CGame::GetInstance()->BoundingBoxDisplayed())
		{
			RenderBoundingBox();
		}

		animationSet->at(0)->Render(x, y);
	}
}

void CCrown::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (!showingEndingEffect)
	{
		CSettingManager* settingManager = CSettingManager::GetInstance();

		left = x;
		top = y;
		right = left + settingManager->GetIntValue("CROWN_BBOX_WIDTH");
		bottom = top + settingManager->GetIntValue("CROWN_BBOX_HEIGHT");
	}
}