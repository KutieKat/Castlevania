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
	locked = false;
	x = 0.0f;
	y = 0.0f;
	width = 0;
	height = 0;
}

void CCamera::SetPosition(float x, float y)
{
	if (!locked)
	{
		this->x = x;
		this->y = y;
	}
}

void CCamera::SetSize(int width, int height)
{
	this->width = width;
	this->height = height;
}

void CCamera::Lock()
{
	locked = true;
}

void CCamera::Unlock()
{
	locked = false;
}

void CCamera::Reset()
{
	x = 0.0f;
	y = 0.0f;
	locked = false;
}

bool CCamera::Locked()
{
	return locked;
}

float CCamera::GetTop()
{
	return y;
}

float CCamera::GetLeft()
{
	return x;
}

float CCamera::GetRight()
{
	return x + width;
}

float CCamera::GetBottom()
{
	return y + height;
}

CCamera::~CCamera()
{
	SAFE_DELETE(instance);
}
