#include "EasterEgg.h"

void CEasterEgg::ShowHiddenItem()
{
	this->hiddenItem->SetState(ITEM_STATE_APPEAR);
}

void CEasterEgg::GetBoundingBox(float & l, float & t, float & r, float & b)
{
	l = x;
	t = y;
	r = x + EASTER_EGG_BBOX_WIDTH;
	b = y + EASTER_EGG_BBOX_HEIGHT;
}

void CEasterEgg::Render()
{
	animations[0]->Render(x, y);
}

void CEasterEgg::SetHiddenItem(CItem* item)
{
	this->hiddenItem = item;
}
