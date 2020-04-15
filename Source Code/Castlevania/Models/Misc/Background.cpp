#include "Background.h"

CBackground::CBackground()
{
	SetAnimationSet("background");
}

void CBackground::Render()
{
	animationSet->at(0)->Render(x, y);
}

void CBackground::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}