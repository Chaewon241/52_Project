#pragma once
#include <vector>

class Sprite
{
public:
	Sprite(std::wstring sheetName, int left, int top, int right, int bottom);
	~Sprite();
	std::wstring m_SpriteName;
	int m_left;
	int m_top;
	int m_right;
	int m_bottom;
	float m_Opacity;
	float m_width;
	float m_height;
};

/*
28, 36, 103, 84,
148, 36, 86, 84,
255, 34, 87, 86,
363, 32, 76, 88,
458, 31, 91, 89,
567, 40, 103, 80
686, 32, 85, 88,
792, 32, 86, 88,
899, 31, 76, 89,
993, 33, 92, 87,
*/