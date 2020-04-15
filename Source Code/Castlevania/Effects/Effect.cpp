#include "Effect.h"
#include "../Animations/AnimationManager.h"
#include "../Animations/AnimationSets.h"

CEffect::CEffect(string type, DWORD timeout)
{
	SetAnimationSet(type);

	this->timeout = timeout;
}

void CEffect::SetAnimationSet(string animationSetId)
{
	this->animationSet = CAnimationSets::GetInstance()->Get(animationSetId);
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
		this->animationSet->at(0)->Render(x, y);
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
