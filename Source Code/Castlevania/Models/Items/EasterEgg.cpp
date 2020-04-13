#include "EasterEgg.h"
#include "../../Utilities/SafeDelete.h"

void CEasterEgg::ShowHiddenItem()
{
	this->hiddenItem->SetState(ITEM_STATE_APPEAR);
	this->hiddenItem->SetDisplayTime(ITEM_DISPLAY_TIME);
}

void CEasterEgg::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + EASTER_EGG_BBOX_WIDTH;
	bottom = top + EASTER_EGG_BBOX_HEIGHT;
}

void CEasterEgg::Render()
{
	animations[0]->Render(x, y);
}

void CEasterEgg::SetHiddenItem(CItem* item)
{
	this->hiddenItem = item;
}

CEasterEgg::~CEasterEgg()
{
	SAFE_DELETE(this->hiddenItem);
}