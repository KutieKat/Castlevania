#pragma once
#include <unordered_map>

#include "GameSound.h"
#include "../Utilities/DSUtil.h"
#include "../Utilities/Debug.h"
using namespace std;

class CGameSoundManager
{
public:
	static CGameSoundManager* GetInstance();

	void Init();
	void Add(string id, string path, bool loop = false, bool retained = false, bool isBackground = false);
	void Remove(string id);
	void Play(string id, bool reset = true);
	void Stop(string id);
	void Clear(bool forced = false);
	void PlayBackgroundSounds();
	void StopBackgroundSounds();

	bool Playing(string id);
	bool Existed(string id);

	CGameSound* GetGameSound(string id);

private:
	static CGameSoundManager* instance;

	unordered_map<string, CGameSound*> sounds;

	CSoundManager* soundManager;
};

