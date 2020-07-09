#include "EasterEgg.h"
#include "../../Utilities/SafeDelete.h"
#include "../../Utilities/Debug.h"

CEasterEgg::CEasterEgg()
{
	SetAnimationSet("transparency");

	mustSit = false;
}

void CEasterEgg::ShowHiddenItem()
{
	CGameObject::ShowHiddenItem();
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

	hiddenItem->SetState(settingManager->GetIntValue("ITEM_STATE_APPEAR"));
	hiddenItem->SetDisplayTime(settingManager->GetIntValue("ITEM_DISPLAY_TIME"));
}

void CEasterEgg::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	CSettingManager* settingManager = CGame::GetInstance()->GetSettingManager();

	left = x;
	top = y;
	right = left + settingManager->GetIntValue("EASTER_EGG_BBOX_WIDTH");
	bottom = top + settingManager->GetIntValue("EASTER_EGG_BBOX_HEIGHT");
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
	animationSet->at(0)->Render(x, y);
}