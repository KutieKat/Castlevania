#include "Color.h"
#include <vector>
#include <sstream>

D3DCOLOR CColor::FromHex(string hex)
{
	int r = HexToDec(hex.substr(1, 2));
	int g = HexToDec(hex.substr(3, 2));
	int b = HexToDec(hex.substr(5, 2));

	return D3DCOLOR_XRGB(r, g, b);
}

D3DCOLOR CColor::FromRgb(string str)
{
	str.replace(str.find("rgb("), 4, "");
	str.replace(str.find(")"), 1, "");

	stringstream ss(str);
	vector<int> rgb;
	int value;

	while (ss >> value) {
		rgb.push_back(value);
		ss.ignore(1);
	}

	return D3DCOLOR_XRGB(rgb[0], rgb[1], rgb[2]);
}

D3DCOLOR CColor::FromRgb(int r, int g, int b)
{
	return D3DCOLOR_XRGB(r, g, b);
}

D3DCOLOR CColor::Parse(string str)
{
	if (str.find("#") == 0)
	{
		return FromHex(str);
	}
	else if (str.find("rgb") == 0)
	{
		return FromRgb(str);
	}
	else
	{
		return White;
	}
}

int CColor::HexToDec(string hex)
{
	int len = hex.length();
	int base = 1;
	int dec = 0;

	for (int i = len - 1; i >= 0; i--)
	{
		if (hex[i] >= '0' && hex[i] <= '9')
		{
			dec += (hex[i] - 48) * base;
		}
		else if (hex[i] >= 'a' && hex[i] <= 'f')
		{
			dec += (hex[i] - 87) * base;
		}
		else if (hex[i] >= 'A' && hex[i] <= 'F')
		{
			dec += (hex[i] - 55) * base;
		}

		base = base * 16;
	}

	return dec;
}
