#include "Timer.h"

CTimer::CTimer()
{
	this->stopped = true;
	this->time = -1;
	this->remainingTime = -1;
}

CTimer::CTimer(DWORD time)
{
	SetTime(time);
}

void CTimer::Start()
{
	this->stopped = false;
}

void CTimer::Stop()
{
	this->stopped = true;
}

void CTimer::SetTime(DWORD time)
{
	Reset();

	this->time = (GetTickCount() / 1000) + time;
}

void CTimer::Tick()
{
	if (this->stopped == false)
	{
		if (this->remainingTime == 0)
		{
			this->stopped = true;
		}
		else
		{
			this->remainingTime = this->time - (GetTickCount() / 1000);
		}

	}
}

void CTimer::Reset()
{
	this->stopped = true;
	this->time = -1;
	this->remainingTime = -1;
}

DWORD CTimer::GetRemainingTime()
{
	return this->remainingTime;
}
