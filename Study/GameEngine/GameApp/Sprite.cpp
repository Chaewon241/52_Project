#include "pch.h"
#include "Sprite.h"

Sprite::Sprite(std::wstring sheetName, int left, int top, int right, int bottom)
	: m_Opacity(1.f)
	, m_height(100.f)
	, m_width(100.f)

{
	m_SpriteName = sheetName;
	m_left = left;
	m_top = top;
	m_right = right + left;
	m_bottom = bottom + top;
}

Sprite::~Sprite()
{
}