#include "Item.h"

void CItem::Disappear()
{
	this->visibility = Visibility::Hidden;
}

void CItem::SetDisplayTime(DWORD time)
{
	this->disappearingTime = GetTickCount() + time * 1000;
}
