#include "IntroBat.h"
#include "../../Utilities/Debug.h"

CIntroBat::CIntroBat()
{
	SetAnimationSet("intro_bat");
	animationSet->at(0)->SetStartTime(GetTickCount());
}

void CIntroBat::Render()
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

void CIntroBat::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}