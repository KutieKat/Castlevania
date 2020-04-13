#pragma once

class CCamera
{
public:
	static CCamera* GetInstance();

	void Init();
	void SetPosition(float x, float y);
	void SetSize(int width, int height);

	float GetTop();
	float GetLeft();

	float GetRight();
	float GetBottom();

	~CCamera();

protected:
	float x;
	float y;

	int width;
	int height;

	static CCamera* instance;
};

