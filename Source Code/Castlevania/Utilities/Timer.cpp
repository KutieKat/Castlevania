#include "Timer.h"

CTimer::CTimer()
{
	paused = true;
}

CTimer::CTimer(DWORD time)
{
	SetTime(time);
}

void CTimer::Start()
{
	paused = false;
}

void CTimer::Pause()
{
	paused = true;
}

void CTimer::SetTime(DWORD time)
{
	endTime = GetTickCount() + time * 1000;
	remainingTime = time;
}

void CTimer::Tick()
{
	if (!paused)
	{
		remainingTime = (endTime - GetTickCount()) / 1000;
	}
}

DWORD CTimer::GetRemainingTime()
{
	return remainingTime;
}
