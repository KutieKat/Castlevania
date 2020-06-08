#include "OneThousand.h"

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
	animationSet->at(0)->Render(x, y);
}
