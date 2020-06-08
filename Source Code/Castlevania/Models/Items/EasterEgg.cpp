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

	hiddenItem->SetState(ITEM_STATE_APPEAR);
}

void CEasterEgg::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
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
	animationSet->at(0)->Render(x, y);
}