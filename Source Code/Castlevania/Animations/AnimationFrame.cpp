#include "AnimationFrame.h"
#include "../../../Utilities/SafeDelete.h"

CAnimationFrame::CAnimationFrame(CSprite* sprite, int time)
{
	this->sprite = sprite;
	this->time = time;
}

DWORD CAnimationFrame::GetTime()
{
	return this->time;
}

CSprite* CAnimationFrame::GetSprite()
{
	return this->sprite;
}

CAnimationFrame::~CAnimationFrame()
{
}
