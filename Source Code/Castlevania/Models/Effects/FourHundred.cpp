#include "FourHundred.h"
#include "../../Game.h"

CFourHundred::CFourHundred()
{
	isEffect = true;
	SetAnimationSet("four_hundred");
	elevation = CSettingManager::GetInstance()->GetIntValue("EFFECT_DEFAULT_ELEVATION");
}

void CFourHundred::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void CFourHundred::Render()
{
	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(0)->Render(x, y);
}
