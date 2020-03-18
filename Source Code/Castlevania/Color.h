#pragma once
#include <string>
#include <d3d9.h>
using namespace std;

class CColor
{
public:
	static D3DCOLOR FromHex(string str);
	static D3DCOLOR FromRgb(string str);
	static D3DCOLOR FromRgb(int r, int g, int b);

	static D3DCOLOR Parse(string str);

	static constexpr D3DCOLOR White = D3DCOLOR_XRGB(255, 255, 255);
	static constexpr D3DCOLOR Black = D3DCOLOR_XRGB(0, 0, 0);

private:
	static int HexToDec(string hex);
};

