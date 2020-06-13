#include "GameSoundManager.h"
#include "../Game.h"
#include "../Utilities/Convert.h"
#include <atlbase.h>

CGameSoundManager* CGameSoundManager::instance = nullptr;

CGameSoundManager* CGameSoundManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CGameSoundManager();
	}

	return instance;
}

void CGameSoundManager::Init()
{
	soundManager = new CSoundManager();
	soundManager->Initialize(CGame::GetInstance()->GetHandler(), DSSCL_PRIORITY);
	soundManager->SetPrimaryBufferFormat(2, 22050, 16);
}

void CGameSoundManager::Add(string id, string path, bool loop, bool retained, bool isBackground)
{
	CSound* sound;
	HRESULT result = soundManager->Create(&sound, CA2T(path.c_str()));

	if (result != DS_OK)
	{
		CDebug::Error("Create sound failed: " + path, "GameSoundManager.cpp");
		return;
	}

	CGameSound* gameSound = new CGameSound(sound, path, loop, retained, isBackground);

	if (isBackground)
	{
		if (Existed(id) == false)
		{
			sounds[id] = gameSound;
		}
		else
		{
			if (sounds[id]->GetPath() != path)
			{
				sounds[id]->Stop();
				sounds[id] = gameSound;
			}
		}
	}
	else
	{
		sounds[id] = gameSound;
	}
}

void CGameSoundManager::Remove(string id)
{
	sounds[id]->Clear();
	sounds.erase(id);
}

void CGameSoundManager::Play(string id, bool reset)
{
	if (reset)
	{
		sounds[id]->Reset();
	}

	sounds[id]->Play();
}

void CGameSoundManager::Stop(string id)
{
	sounds[id]->Stop();
}

void CGameSoundManager::Clear(bool forced)
{
	if (!forced)
	{
		unordered_map<std::string, CGameSound*>::const_iterator sound = sounds.begin();

		while (sound != sounds.end())
		{
			if (sound->second->Retained() == false)
			{
				sound->second->Clear();
				sound = sounds.erase(sound);
			}
			else
			{
				sound->second->Stop();
			}

			if (sound != sounds.end())
			{
				sound++;
			}

		}
	}
	else
	{
		for (auto sound : sounds)
		{
			sound.second->Clear();
		}

		sounds.clear();
	}
}

void CGameSoundManager::PlayBackgroundSounds()
{
	unordered_map<std::string, CGameSound*>::const_iterator sound;

	for (sound = sounds.begin(); sound != sounds.end(); ++sound)
	{
		if (sound->second->IsBackground())
		{
			sound->second->Play();
			break;
		}
	}
}

void CGameSoundManager::StopBackgroundSounds()
{
	for (auto sound : sounds)
	{
		if (sound.second->IsBackground())
		{
			sound.second->Stop();
		}
	}
}

bool CGameSoundManager::Playing(string id)
{
	return sounds[id]->Playing();
}

CGameSound* CGameSoundManager::GetGameSound(string id)
{
	return sounds[id];
}

bool CGameSoundManager::Existed(string id)
{
	if (sounds.find(id) != sounds.end())
	{
		return true;
	}

	return false;
}
