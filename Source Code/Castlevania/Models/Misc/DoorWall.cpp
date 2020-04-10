#include "DoorWall.h"

void CDoorWall::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + DOOR_WALL_BBOX_WIDTH;
	bottom = y + DOOR_WALL_BBOX_HEIGHT;
}

void CDoorWall::Render()
{
	animations[0]->Render(x, y);
}
