#pragma once

class CCamera
{
public:
	static CCamera* GetInstance();

	void Init();
	void SetPosition(float x, float y);
	void SetSize(int width, int height);

	void Lock();
	void Unlock();

	bool Locked();

	float GetTop();
	float GetLeft();

	float GetRight();
	float GetBottom();

	~CCamera();

protected:
	bool locked;

	float x;
	float y;

	int width;
	int height;

	static CCamera* instance;
};

