#pragma once
#include <d3d9.h>
using namespace std;

class CTimer
{
public:
	CTimer();
	CTimer(DWORD time);

	void Resume();
	void Pause();
	void SetTime(DWORD time);
	void Tick();
	void Decrease();

	DWORD GetRemainingTime();

protected:
	DWORD remainingTime;

	bool paused;
	int counter;
};

