#pragma once

#include <Windows.h>

namespace catInWonderland
{
	struct Sprite
	{
	public:
		Sprite() = default;
		Sprite(HDC hdc, HBITMAP bitmap, BITMAP bitInfo);
		~Sprite();

	public:
		HDC Hdc;
		HBITMAP Bitmap;
		BITMAP BitInfo;
	};
}