#include "AnimationFrame.h"
#include "../../../Utilities/SafeDelete.h"

CAnimationFrame::CAnimationFrame(CSprite* sprite, int time)
{
	this->sprite = sprite;
	this->time = time;
}

DWORD CAnimationFrame::GetTime()
{
	return time;
}

CSprite* CAnimationFrame::GetSprite()
{
	return sprite;
}

CAnimationFrame::~CAnimationFrame()
{
}
