#include "EasterEgg.h"

CEasterEgg::CEasterEgg(CItem* hiddenItem)
{
	this->hiddenItem = hiddenItem;
}

void CEasterEgg::ShowHiddenItem()
{
	this->hiddenItem->SetState(ITEM_STATE_APPEAR);
}

void CEasterEgg::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + EASTER_EGG_BBOX_WIDTH;
	bottom = top + EASTER_EGG_BBOX_HEIGHT;
}

CBoundingBox CEasterEgg::GetBoundingBox()
{
	CBoundingBox boundingBox;

	GetBoundingBox(boundingBox.left, boundingBox.top, boundingBox.right, boundingBox.bottom);

	return boundingBox;
}

void CEasterEgg::Render()
{
	animations[0]->Render(x, y);
}
