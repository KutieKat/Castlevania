#include "Timer.h"
#include "../Game.h"

CTimer::CTimer()
{
	paused = true;
	remainingTime = -1;
	counter = 0;
}

CTimer::CTimer(DWORD time)
{
	SetTime(time);
}

void CTimer::Resume()
{
	paused = false;
}

void CTimer::Pause()
{
	paused = true;
}

void CTimer::SetTime(DWORD time)
{
	remainingTime = time;
}

void CTimer::Tick()
{
	if (!paused && remainingTime > 0)
	{
		counter++;

		if (counter % 70 == 0)
		{
			if (remainingTime <= 10)
			{
				CGame::GetInstance()->GetSoundManager()->Play("ticking");
			}
			else
			{
				CGame::GetInstance()->GetSoundManager()->Stop("ticking");
			}

			remainingTime -= 1;
		}
	}
}

void CTimer::Decrease()
{
	remainingTime -= 1;
}

DWORD CTimer::GetRemainingTime()
{
	return remainingTime;
}
