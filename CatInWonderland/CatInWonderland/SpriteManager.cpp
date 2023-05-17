#include <fstream>
#include <string>

#include "RenderManager.h"
#include "SpriteManager.h"
#include "Sprite.h"

namespace catInWonderland
{
	SpriteManager* SpriteManager::mInstance = nullptr;

	SpriteManager* SpriteManager::GetInstance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new SpriteManager();
		}

		return mInstance;
	}

	void SpriteManager::DestroyInstance()
	{
		delete mInstance;
		mInstance = nullptr;
	}

	SpriteManager::~SpriteManager()
	{
		for (auto iter = mSpriteMap.begin(); iter != mSpriteMap.end(); ++iter)
		{
			delete iter->second;
		}
	}

	void SpriteManager::LoadSpriteImage(eSpriteType spriteType, const WCHAR* fileName)
	{
		Sprite* sprite = new Sprite;
		sprite->Hdc = CreateCompatibleDC(RenderManager::GetInstance()->GetFrontDC());
		sprite->Bitmap = (HBITMAP)LoadImageW(nullptr, fileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		assert(sprite->Bitmap != nullptr);
		HBITMAP prevBitmap = (HBITMAP)SelectObject(sprite->Hdc, sprite->Bitmap);
		DeleteObject(prevBitmap);

		GetObject(sprite->Bitmap, sizeof(BITMAP), &sprite->BitInfo);

		mSpriteMap.emplace(spriteType, sprite);
	}

	void SpriteManager::LoadSpriteImage(eSpriteType spriteType, const char* fileName)
	{
		Sprite* sprite = new Sprite;
		sprite->Hdc = CreateCompatibleDC(RenderManager::GetInstance()->GetFrontDC());
		sprite->Bitmap = (HBITMAP)LoadImageA(nullptr, fileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
		assert(sprite->Bitmap != nullptr);
		HBITMAP prevBitmap = (HBITMAP)SelectObject(sprite->Hdc, sprite->Bitmap);
		DeleteObject(prevBitmap);

		GetObject(sprite->Bitmap, sizeof(BITMAP), &sprite->BitInfo);

		mSpriteMap.emplace(spriteType, sprite);
	}

	void SpriteManager::LoadAnimationRectangle(eAnimationType animationType, const char* fileName)
	{
		std::fstream fin;
		fin.open(fileName, std::ios_base::in);

		/*hRect
		std::string garbage;


		fin >> sp->Width;
		getline(fin, garbage);

		int curPoint = fin.tellg();
		fileSize -= curPoint;

		sp->Height = fileSize / sp->Width;
		char* img = new char[fileSize];
		fin.read(img, fileSize);

		sp->Img = img;
		mSpriteMap.insert({ spriteType, sp });

		fin.close();*/
	}
}