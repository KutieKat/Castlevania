#pragma once
#include <d3d9.h>

class CTimer
{
public:
	CTimer();
	CTimer(DWORD time);

	void Start();
	void Stop();
	void SetTime(DWORD time);
	void Tick();
	void Reset();

	DWORD GetRemainingTime();

protected:
	DWORD time;
	DWORD duration;
	DWORD remainingTime;

	bool stopped;
};

