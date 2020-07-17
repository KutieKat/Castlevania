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

void CGameSoundManager::Add(string id, string path, bool loop, bool retained, bool isBackground, bool sharable)
{
	CSound* sound;
	HRESULT result = soundManager->Create(&sound, CA2T(path.c_str()));

	if (result != DS_OK)
	{
		CDebug::Error("Create sound failed: " + path, "GameSoundManager.cpp");
		return;
	}

	CGameSound* gameSound = new CGameSound(sound, path, loop, retained, isBackground);

	if (sharable)
	{
		sharedSounds[id] = gameSound;
	}
	else
	{
		if (isBackground)
		{
			if (Existed(id) == false)
			{
				sounds[id] = gameSound;
			}
			else
			{
				if (sounds[id] != nullptr && sounds[id]->GetPath() != path)
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
}

void CGameSoundManager::Play(string id, bool reset)
{
	bool foundInSounds = sounds[id] != nullptr;
	bool foundInSharedSounds = sharedSounds[id] != nullptr;

	if (foundInSounds || foundInSharedSounds)
	{
		if (foundInSounds)
		{
			if (reset)
			{
				sounds[id]->Reset();
			}

			sounds[id]->Play();
		}
		else
		{
			if (reset)
			{
				sharedSounds[id]->Reset();
			}

			sharedSounds[id]->Play();
		}
	}
	else
	{
		CDebug::Error("Failed to find sound id=" + id, "GameSoundManager.cpp");
	}
}

void CGameSoundManager::Stop(string id)
{
	bool foundInSounds = sounds[id] != nullptr;
	bool foundInSharedSounds = sharedSounds[id] != nullptr;

	if (foundInSounds || foundInSharedSounds)
	{
		if (foundInSounds)
		{
			sounds[id]->Stop();
		}
		else
		{
			sharedSounds[id]->Stop();
		}
	}
	else
	{
		CDebug::Error("Failed to find sound id=" + id, "GameSoundManager.cpp");
	}
}

void CGameSoundManager::Clear(bool forced)
{
	if (!forced)
	{
		unordered_map<std::string, CGameSound*>::const_iterator sound = sounds.begin();

		while (sound != sounds.end())
		{
			if (sound->second != nullptr)
			{
				if (sound->second->Retained() == false)
				{
					sound->second->Stop();
					sound->second->Clear();
					sound = sounds.erase(sound);
				}
				else
				{
					sound->second->Stop();
				}
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
			if (sound.second != nullptr)
			{
				sound.second->Clear();
			}
		}

		sounds.clear();
	}

}

void CGameSoundManager::PlayBackgroundSounds()
{
	unordered_map<std::string, CGameSound*>::const_iterator sound;

	for (sound = sounds.begin(); sound != sounds.end(); ++sound)
	{
		if (sound->second != nullptr && sound->second->IsBackground())
		{
			sound->second->Play();
			break;
		}
	}

	//CDebug::Info("Sounds rieng: " + to_string(sounds.size()) + ", Sounds chung: " + to_string(sharedSounds.size()));
}

void CGameSoundManager::StopBackgroundSounds()
{
	for (auto sound : sounds)
	{
		if (sound.second != nullptr && sound.second->IsBackground())
		{
			sound.second->Stop();
		}
	}

	for (auto sound : sharedSounds)
	{
		if (sound.second != nullptr && sound.second->IsBackground())
		{
			sound.second->Stop();
		}
	}
}

bool CGameSoundManager::Playing(string id)
{
	bool foundInSounds = sounds[id] != nullptr;
	bool foundInSharedSounds = sharedSounds[id] != nullptr;

	if (foundInSounds || foundInSharedSounds)
	{
		if (foundInSounds)
		{
			return sounds[id]->Playing();
		}
		else
		{
			return sharedSounds[id]->Playing();
		}
	}
	else
	{
		CDebug::Error("Failed to find sound id=" + id, "GameSoundManager.cpp");
		return false;
	}
}

CGameSound* CGameSoundManager::GetGameSound(string id)
{
	bool foundInSounds = sounds[id] != nullptr;
	bool foundInSharedSounds = sharedSounds[id] != nullptr;

	if (foundInSounds || foundInSharedSounds)
	{
		if (foundInSounds)
		{
			return sounds[id];
		}
		else
		{
			return sharedSounds[id];
		}
	}
	else
	{
		CDebug::Error("Failed to find sound id=" + id, "GameSoundManager.cpp");
	}
}

bool CGameSoundManager::Existed(string id)
{
	if (sounds.find(id) != sounds.end() || sharedSounds.find(id) != sharedSounds.end())
	{
		return true;
	}

	return false;
}
