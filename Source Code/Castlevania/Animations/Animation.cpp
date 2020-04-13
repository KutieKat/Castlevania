#include "Animation.h"
#include "../Sprites/SpriteManager.h"
#include "../Sprites/Sprite.h"
#include "../Utilities/Constants.h"

CAnimation::CAnimation(int defaultTime)
{
	this->defaultTime = defaultTime;
	this->lastFrameTime = -1;
	this->currentFrame = -1;
	this->endTime = -1;
}

void CAnimation::Add(string spriteId, DWORD time)
{
	int t = time;

	if (time == 0)
	{
		t = this->defaultTime;
	}

	CSprite* sprite = CSpriteManager::GetInstance()->Get(spriteId);
	CAnimationFrame* frame = new CAnimationFrame(sprite, t);

	frames.push_back(frame);
}

void CAnimation::Render(float x, float y, int alpha)
{
	DWORD now = GetTickCount();

	if (currentFrame == -1)
	{
		currentFrame = 0;
		lastFrameTime = now;
	}
	else
	{
		DWORD t = frames[currentFrame]->GetTime();

		if (now - lastFrameTime > t)
		{
			currentFrame++;
			lastFrameTime = now;

			if (currentFrame == frames.size())
			{
				currentFrame = 0;
			}
		}
	}

	frames[currentFrame]->GetSprite()->Draw(x, y, alpha);
}

int CAnimation::GetCurrentFrame()
{
	return this->currentFrame;
}

int CAnimation::GetTotalFrames()
{
	return this->frames.size();
}

void CAnimation::SetStartTime(DWORD start)
{
	DWORD animationTime = 0;

	for (int i = 0; i < this->frames.size(); i++)
	{
		animationTime += this->frames[i]->GetTime();
	}

	this->endTime = start + animationTime;
}

bool CAnimation::Over()
{
	return GetTickCount() > this->endTime;
}

void CAnimation::Reset()
{
	this->lastFrameTime = -1;
	this->currentFrame = -1;
}