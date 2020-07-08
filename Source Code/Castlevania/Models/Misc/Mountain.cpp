#include "Mountain.h"
#include "../../Game.h"

CMountain::CMountain()
{
	SetAnimationSet("mountain");

	elevation = 2;
}

void CMountain::Render()
{
	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(0)->Render(x, y);
}

void CMountain::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}