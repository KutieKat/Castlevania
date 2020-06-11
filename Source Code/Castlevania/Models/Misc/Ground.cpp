#include "Ground.h"

CGround::CGround()
{
	SetAnimationSet("transparency");
}

void CGround::Render()
{
	animationSet->at(0)->Render(x, y);
}

void CGround::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = left + width;
	bottom = top + height;
}

void CGround::SetWidth(float width)
{
	this->width = width;
}

void CGround::SetHeight(float height)
{
	this->height = height;
}
