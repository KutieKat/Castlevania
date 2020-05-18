#include "BigDoubleShot.h"
#include "../../Utilities/Debug.h"

CBigDoubleShot::CBigDoubleShot()
{
	SetAnimationSet("big_double_shot");
	animationSet->at(0)->SetStartTime(GetTickCount());
}

void CBigDoubleShot::Render()
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

void CBigDoubleShot::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}