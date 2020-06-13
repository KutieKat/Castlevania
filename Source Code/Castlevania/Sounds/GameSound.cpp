#include "GameSound.h"

CGameSound::CGameSound(CSound* sound, string path, bool loop, bool retained, bool isBackground)
{
	this->sound = sound;
	this->path = path;
	this->loop = loop;
	this->retained = retained;
	this->isBackground = isBackground;
}

void CGameSound::Play()
{
	sound->Play(0, loop);
}

void CGameSound::Stop()
{
	sound->Stop();
}

void CGameSound::Reset()
{
	sound->Reset();
}

void CGameSound::Clear()
{
	SAFE_DELETE(sound);
}

bool CGameSound::Playing()
{
	return sound->IsSoundPlaying();
}

bool CGameSound::Retained()
{
	return retained;
}

bool CGameSound::IsBackground()
{
	return isBackground;
}

string CGameSound::GetPath()
{
	return path;
}

CGameSound::~CGameSound()
{
	Clear();
}
