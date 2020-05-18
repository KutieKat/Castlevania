#include "BigTripleShot.h"

CBigTripleShot::CBigTripleShot()
{
	SetAnimationSet("big_triple_shot");
	animationSet->at(0)->SetStartTime(GetTickCount());
}

void CBigTripleShot::Render()
{
	if (animationSet->at(0)->Over())
	{
		animationSet->at(1)->Render(x, y);
	}
	else
	{
		animationSet->at(0)->Render(x, y);
	}
}

void CBigTripleShot::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}