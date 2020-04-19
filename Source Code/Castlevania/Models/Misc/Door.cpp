#include "Door.h"

CDoor::CDoor()
{
	SetAnimationSet("transparency");
}

void CDoor::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + DOOR_BBOX_WIDTH;
	bottom = top + DOOR_BBOX_HEIGHT;
}

void CDoor::Render()
{
	animationSet->at(0)->Render(x, y);
}