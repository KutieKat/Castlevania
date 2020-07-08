#include "OneThousand.h"
#include "../../Game.h"

COneThousand::COneThousand()
{
	isEffect = true;
	SetAnimationSet("one_thousand");
	elevation = EFFECT_DEFAULT_ELEVATION;
}

void COneThousand::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void COneThousand::Render()
{
	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(0)->Render(x, y);
}
