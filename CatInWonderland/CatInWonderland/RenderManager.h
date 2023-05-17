#pragma once

#include <Windows.h>

namespace catInWonderland
{
	class hRectangle;
	class Sprite;

	class RenderManager
	{
	public:
		static RenderManager* GetInstance();
		static void DestroyInstance();

		void Render();

		void Draw(const hRectangle& worldRectangle, const hRectangle& spriteRectangle, const Sprite& sprite);

		inline HDC GetFrontDC() const;

	private:
		RenderManager();
		~RenderManager();

	private:
		static RenderManager* mInstance;

		HDC mFrontMemDC;
		HDC mBackMemDC;
		HDC mRotateMemDC;

		HBITMAP mBackBitmap;
		HBITMAP mRotateBitmap;
	};

	HDC RenderManager::GetFrontDC() const
	{
		return mFrontMemDC;
	}
}