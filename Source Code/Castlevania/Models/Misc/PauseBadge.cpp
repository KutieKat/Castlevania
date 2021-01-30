#include "PauseBadge.h"
#include "../../Game.h"

CPauseBadge::CPauseBadge()
{
	SetAnimationSet("paused");
}

void CPauseBadge::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void CPauseBadge::Render()
{
	animationSet->at(0)->Render(x, y);
}
