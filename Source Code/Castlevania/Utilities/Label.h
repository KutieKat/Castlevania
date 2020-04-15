#pragma once
#include <string>
#include <d3d9.h>

#include "Color.h"
using namespace std;

struct ID3DXFont;

class CLabel
{
public:
	CLabel(string text, float x = 0.0f, float y = 0.0f, float fontSize = 14, LPWSTR fontFamily = L"Press Start", D3DCOLOR color = CColor::FromRgb(255, 255, 255));

	void SetText(string text);
	void SetColor(D3DCOLOR color);
	void SetPosition(float x, float y);
	void Render();

	~CLabel();

protected:
	string text;
	LPWSTR fontFamily;

	D3DCOLOR color;
	ID3DXFont* font;

	RECT rect;

	float fontSize;

	float left;
	float top;
	float right;
	float bottom;

	float calcRightPosition();
	float calcBottomPosition();

	void ReCalcPosition();
	void UpdateFont();
};

