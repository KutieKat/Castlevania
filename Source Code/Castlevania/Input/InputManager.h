#pragma once
#include <d3d9.h>
#include <dinput.h>

#include "IKeyEventHandler.h"
#include "../Utilities/Constants.h"

class CInputManager
{
public:
	static CInputManager* GetInstance();

	void Init(HWND hWnd);
	void SetKeyHandler(IKeyEventHandler* keyHandler);
	void ProcessKeyboard();

	int IsKeyDown(int keyCode);

	~CInputManager();

private:
	LPDIRECTINPUT8 di;
	LPDIRECTINPUTDEVICE8 didv;

	BYTE keyStates[256];
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];

	IKeyEventHandler* keyHandler;

	static CInputManager* instance;
};

