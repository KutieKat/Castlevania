#include "Door.h"
#include "../../Utilities/SafeDelete.h"

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

void CDoor::SetDoorWall(CDoorWall* doorWall)
{
	this->doorWall = doorWall;
}

CDoorWall* CDoor::GetDoorWall()
{
	return this->doorWall;
}

void CDoor::SetNextSceneId(string sceneId)
{
	this->nextSceneId = sceneId;
}

string CDoor::GetNextSceneId()
{
	return this->nextSceneId;
}

CDoor::~CDoor()
{
	SAFE_DELETE(this->doorWall);
}
