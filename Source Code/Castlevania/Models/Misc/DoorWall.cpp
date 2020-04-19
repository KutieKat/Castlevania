#include "DoorWall.h"

CDoorWall::CDoorWall()
{
	comeThrough = true;

	SetAnimationSet("door_wall");
}

void CDoorWall::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = left + DOOR_WALL_BBOX_WIDTH;
	bottom = top + DOOR_WALL_BBOX_HEIGHT;
}

void CDoorWall::Render()
{
	animationSet->at(0)->Render(x, y);
}
