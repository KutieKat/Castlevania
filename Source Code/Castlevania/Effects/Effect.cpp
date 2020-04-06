#include "Effect.h"
#include "../Animations/AnimationManager.h"
#include "../Utilities/Debug.h"

void CEffect::AddAnimation(string aniId)
{
	CAnimation* ani = CAnimationManager::GetInstance()->Get(aniId);
	animations.push_back(ani);
}

void CEffect::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void CEffect::GetPosition(float & x, float & y)
{
	x = this->x;
	y = this->y;
}

void CEffect::SetTimeout(DWORD timeout)
{
	this->timeout = timeout;
}

void CEffect::SetStartTime(DWORD startTime)
{
	this->startTime = GetTickCount();
}

void CEffect::Render()
{
	if (!Over())
	{
		this->animations[0]->Render(x, y);
	}
}

DWORD CEffect::GetStartTime()
{
	return this->startTime;
}

bool CEffect::Over()
{
	return this->startTime != -1 && GetTickCount() - this->startTime > this->timeout;
}
