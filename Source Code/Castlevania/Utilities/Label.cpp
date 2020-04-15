#include "Label.h"
#include "../Game.h"

CLabel::CLabel(string text, float x, float y, float fontSize, LPWSTR fontFamily, D3DCOLOR color)
{
	this->text = text;
	this->left = x;
	this->top = y;
	this->fontSize = fontSize;
	this->fontFamily = fontFamily;
	this->color = color;
	this->right = calcRightPosition();
	this->bottom = calcBottomPosition();

	UpdateFont();
}

void CLabel::SetText(string text)
{
	this->text = text;

	ReCalcPosition();
}

void CLabel::SetColor(D3DCOLOR color)
{
	this->color = color;
}

void CLabel::SetPosition(float x, float y)
{
	this->left = x;
	this->top = y;

	ReCalcPosition();
}

void CLabel::Render()
{
	SetRect(&rect, this->left, this->top, this->right, this->bottom);
	font->DrawTextA(nullptr, this->text.c_str(), -1, &rect, DT_LEFT, this->color);
}

float CLabel::calcRightPosition()
{
	return this->left + this->text.length() * this->fontSize;
}

float CLabel::calcBottomPosition()
{
	return this->top + this->fontSize;
}

void CLabel::ReCalcPosition()
{
	this->right = calcRightPosition();
	this->bottom = calcBottomPosition();
}

void CLabel::UpdateFont()
{
	D3DXCreateFont(
		CGame::GetInstance()->GetDirect3DDevice(),
		this->fontSize,
		0,
		FW_NORMAL,
		1,
		false,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FF_DONTCARE,
		this->fontFamily,
		&font
	);
}

CLabel::~CLabel()
{
	font->Release();
}
