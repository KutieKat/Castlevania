#include "EasterEgg.h"
#include "../../Game.h"

CEasterEgg::CEasterEgg()
{
	SetAnimationSet("transparency");

	mustSit = false;
}

void CEasterEgg::ShowHiddenItem()
{
	CGameObject::ShowHiddenItem();
	CSettingManager* settingManager = CSettingManager::GetInstance();

	hiddenItem->SetState(ITEM_STATE_APPEAR);
	hiddenItem->SetDisplayTime(settingManager->GetIntValue("ITEM_DISPLAY_TIME"));
}

void CEasterEgg::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	CSettingManager* settingManager = CSettingManager::GetInstance();

	left = x;
	top = y;
	right = left + EASTER_EGG_BBOX_WIDTH;
	bottom = top + EASTER_EGG_BBOX_HEIGHT;
}

void CEasterEgg::SetMustSit(bool mustSit)
{
	this->mustSit = mustSit;
}

bool CEasterEgg::MustSit()
{
	return mustSit;
}

void CEasterEgg::Render()
{
	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(0)->Render(x, y);
}