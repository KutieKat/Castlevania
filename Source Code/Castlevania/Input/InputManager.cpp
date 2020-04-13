#include "InputManager.h"
#include "../Utilities/Debug.h"
#include "../Utilities/SafeDelete.h"

CInputManager* CInputManager::instance = nullptr;

CInputManager* CInputManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new CInputManager();
	}

	return instance;
}

void CInputManager::Init(HWND hWnd, IKeyEventHandler* keyHandler)
{
	HRESULT
		hr = DirectInput8Create
		(
		(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),
			DIRECTINPUT_VERSION,
			IID_IDirectInput8, (VOID**)&di, NULL
		);

	if (hr != DI_OK)
	{
		int value = 1;
		CDebug::Error("DirectInput8Create failed!", "InputManager.cpp");
		return;
	}

	hr = di->CreateDevice(GUID_SysKeyboard, &didv, NULL);

	if (hr != DI_OK)
	{
		CDebug::Error("CreateDevice failed!", "InputManager.cpp");
		return;
	}

	hr = didv->SetDataFormat(&c_dfDIKeyboard);
	hr = didv->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);

	DIPROPDWORD dipdw;

	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE; // Arbitary buffer size

	hr = didv->SetProperty(DIPROP_BUFFERSIZE, &dipdw.diph);

	hr = didv->Acquire();
	if (hr != DI_OK)
	{
		CDebug::Error("DINPUT8::Acquire failed!", "InputManager.cpp");
		return;
	}

	this->keyHandler = keyHandler;

	CDebug::Info("Keyboard has been initialized successfully!", "InputManager.cpp");
}

void CInputManager::ProcessKeyboard()
{
	HRESULT hr;

	hr = didv->GetDeviceState(sizeof(keyStates), keyStates);
	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			HRESULT h = didv->Acquire();
			if (h == DI_OK)
			{
				CDebug::Info("Keyboard re-acquired!", "InputManager.cpp");
			}
			else return;
		}
		else
		{
			return;
		}
	}

	keyHandler->KeyState((BYTE *)&keyStates);

	// Collect all buffered events
	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	hr = didv->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), keyEvents, &dwElements, 0);

	if (FAILED(hr))
	{
		//DebugOut(L"[ERROR] DINPUT::GetDeviceData failed. Error: %d\n", hr);
		return;
	}

	for (DWORD i = 0; i < dwElements; i++)
	{
		int KeyCode = keyEvents[i].dwOfs;
		int KeyState = keyEvents[i].dwData;

		if ((KeyState & 0x80) > 0)
			keyHandler->OnKeyDown(KeyCode);
		else
			keyHandler->OnKeyUp(KeyCode);
	}
}

int CInputManager::IsKeyDown(int keyCode)
{
	return (keyStates[keyCode] & 0x80) > 0;
}

CInputManager::~CInputManager()
{
	SAFE_DELETE(instance);
}
