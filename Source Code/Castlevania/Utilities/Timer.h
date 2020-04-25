#pragma once
#include <d3d9.h>
using namespace std;

class CTimer
{
public:
	CTimer();
	CTimer(DWORD time);

	void Start();
	void Pause();
	void SetTime(DWORD time);
	void Tick();

	DWORD GetRemainingTime();

protected:
	DWORD endTime;
	DWORD remainingTime;

	bool paused;
};

