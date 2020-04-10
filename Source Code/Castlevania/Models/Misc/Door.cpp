#include "Door.h"

void CDoor::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
	left = x;
	top = y;
	right = x + DOOR_BBOX_WIDTH;
	bottom = y + DOOR_BBOX_HEIGHT;
}

void CDoor::Render()
{
	animations[0]->Render(x, y);
}

void CDoor::SetDoorWall(CDoorWall* doorWall)
{
	this->doorWall = doorWall;
}

CDoorWall* CDoor::GetDoorWall()
{
	return this->doorWall;
}
