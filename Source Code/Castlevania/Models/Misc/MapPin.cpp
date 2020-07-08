#include "MapPin.h"
#include "../../Game.h"

CMapPin::CMapPin()
{
	SetAnimationSet("map_pin");
}

void CMapPin::Render()
{
	if (CGame::GetInstance()->BoundingBoxDisplayed())
	{
		RenderBoundingBox();
	}

	animationSet->at(0)->Render(x, y);
}

void CMapPin::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}