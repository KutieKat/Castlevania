#include "Background.h"
#include "../../Game.h"

CBackground::CBackground()
{
	SetAnimationSet("background");
}

void CBackground::Render()
{
	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(0)->Render(x, y);
}

void CBackground::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}