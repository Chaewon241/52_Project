#include "RenderManager.h"
#include "SpriteManager.h"
#include "Sprite.h"
#include "WinApp.h"
#include "Vector2.h"

#pragma comment(lib, "msimg32.lib")

namespace catInWonderland
{
	RenderManager* RenderManager::mInstance = nullptr;

	RenderManager* RenderManager::GetInstance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new RenderManager();
		}

		return mInstance;
	}

	void RenderManager::DestroyInstance()
	{
		delete mInstance;
		mInstance = nullptr;
	}

	RenderManager::RenderManager()
	{
		mFrontMemDC = GetDC(WinApp::GetWindow());	// 윈도우에 그림을 그리는 DC 얻기
		mBackMemDC = CreateCompatibleDC(mFrontMemDC);	// frontMemDC와 호환되는 DC를 생성
		mRotateMemDC = CreateCompatibleDC(mRotateMemDC);	// frontMemDC와 호환되는 DC를 생성

		mBackBitmap = CreateCompatibleBitmap(mFrontMemDC, WinApp::GetWidth(), WinApp::GetHeight());
		mRotateBitmap = CreateCompatibleBitmap(mFrontMemDC, WinApp::GetWidth(), WinApp::GetHeight());

		SelectObject(mBackMemDC, mBackBitmap);
		SelectObject(mRotateMemDC, mRotateBitmap);
	}

	RenderManager::~RenderManager()
	{
		DeleteObject(mBackBitmap);
		DeleteObject(mRotateBitmap);
		DeleteDC(mBackMemDC);
		DeleteDC(mRotateMemDC);
		ReleaseDC(WinApp::GetWindow(), mFrontMemDC);
	}

	void RenderManager::Render()
	{
		BitBlt(mFrontMemDC, 0, 0, WinApp::GetWidth(), WinApp::GetHeight(), mBackMemDC, 0, 0, SRCCOPY);
		PatBlt(mBackMemDC, 0, 0, WinApp::GetWidth(), WinApp::GetHeight(), WHITENESS);
	}

	void RenderManager::Draw(const hRectangle& worldRectangle, const hRectangle& spriteRectangle, const Sprite& sprite)
	{
		const Vector2& topLeft = worldRectangle.GetTopLeft();
		const Vector2& topRight = worldRectangle.GetTopRight();
		const Vector2& bottomLeft = worldRectangle.GetBottomLeft();

		POINT points[3] = { 0, };

		points[0] = { static_cast<long>(topLeft.GetX()), static_cast<long>(topLeft.GetY()) };
		points[1] = { static_cast<long>(topRight.GetX()), static_cast<long>(topRight.GetY()) };
		points[2] = { static_cast<long>(bottomLeft.GetX()), static_cast<long>(bottomLeft.GetY()) };

		const Vector2& spritePosition = spriteRectangle.GetTopLeft();
		const Vector2& spriteSize = spriteRectangle.GetSize();

		PlgBlt(mRotateMemDC, points, sprite.Hdc, spritePosition.GetX(), spritePosition.GetY(), spriteSize.GetX(), spriteSize.GetY(), 0, 0, 0);

		const hRectangle boundingRectangle = hRectangle::GetBoundingRectangle(worldRectangle);
		const Vector2& boundingTopLeft = boundingRectangle.GetTopLeft();
		const Vector2& boundingBottomRight = boundingRectangle.GetBottomRight();

		TransparentBlt(mBackMemDC,
			static_cast<int>(boundingTopLeft.GetX()),
			static_cast<int>(boundingTopLeft.GetY()),
			static_cast<int>(boundingBottomRight.GetX() - boundingTopLeft.GetX()),
			static_cast<int>(boundingBottomRight.GetY() - boundingTopLeft.GetY()),
			mRotateMemDC,
			static_cast<int>(boundingTopLeft.GetX()),
			static_cast<int>(boundingTopLeft.GetY()),
			static_cast<int>(boundingBottomRight.GetX() - boundingTopLeft.GetX()),
			static_cast<int>(boundingBottomRight.GetY() - boundingTopLeft.GetY()),
			RGB(255, 255, 255));

		PatBlt(mRotateMemDC, 0, 0, WinApp::GetWidth(), WinApp::GetHeight(), WHITENESS);
	}
}