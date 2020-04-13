#include "Camera.h"
#include "../Utilities/SafeDelete.h"

CCamera* CCamera::instance = nullptr;

CCamera* CCamera::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CCamera();
	}

	return instance;
}

void CCamera::Init()
{
	this->x = 0.0f;
	this->y = 0.0f;
	this->width = 0;
	this->height = 0;
}

void CCamera::SetPosition(float x, float y)
{
	this->x = x;
	this->y = y;
}

void CCamera::SetSize(int width, int height)
{
	this->width = width;
	this->height = height;
}

float CCamera::GetTop()
{
	return this->y;
}

float CCamera::GetLeft()
{
	return this->x;
}

float CCamera::GetRight()
{
	return this->x + this->width;
}

float CCamera::GetBottom()
{
	return this->y + this->height;
}

CCamera::~CCamera()
{
	SAFE_DELETE(instance);
}
