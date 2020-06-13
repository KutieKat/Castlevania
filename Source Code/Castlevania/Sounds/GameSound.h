#pragma once
#include <string>

#include "../Utilities/DSUtil.h"
#include "../Utilities/SafeDelete.h"
using namespace std;

class CGameSound
{
public:
	CGameSound(CSound* sound, string path, bool loop = false, bool retained = false, bool isBackground = false);

	void Play();
	void Stop();
	void Reset();
	void Clear();

	bool Playing();
	bool Retained();
	bool IsBackground();

	string GetPath();

	~CGameSound();

private:
	CSound* sound;

	bool loop;
	bool retained;
	bool isBackground;

	string path;
};

