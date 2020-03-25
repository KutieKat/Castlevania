#pragma once
#include <d3d9.h>

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#define KEYBOARD_BUFFER_SIZE 1024

#include "IKeyEventHandler.h"

class CInputManager
{
public:
	static CInputManager* GetInstance();

	void Init(HWND hWnd, IKeyEventHandler* keyHandler);
	void ProcessKeyboard();

	int IsKeyDown(int keyCode);

private:
	LPDIRECTINPUT8 di;
	LPDIRECTINPUTDEVICE8 didv;

	BYTE keyStates[256];
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];

	IKeyEventHandler* keyHandler;

	static CInputManager* instance;
};

