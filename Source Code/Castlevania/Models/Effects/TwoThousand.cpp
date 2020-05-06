#include "TwoThousand.h"

CTwoThousand::CTwoThousand()
{
	isEffect = true;
	SetAnimationSet("two_thousand");
}

void CTwoThousand::GetBoundingBox(float & left, float & top, float & right, float & bottom)
{
}

void CTwoThousand::Render()
{
	animationSet->at(0)->Render(x, y);
}
