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

	void SpriteManager::LoadAnimationRectangle(eAnimationType animationType, const WCHAR* fileName)
	{
		std::ifstream fin;
		fin.open(fileName, std::ios_base::in);

		assert(fin.is_open());

		float x1;
		float y1;
		float x2;
		float y2;

		mSpriteRectMap.emplace(animationType, std::vector<hRectangle>());
		std::string trash;

		while (true)
		{
			fin >> x1;
			fin >> y1;
			fin >> x2;
			fin >> y2;

			if (!fin.fail())
			{
				mSpriteRectMap[animationType].push_back(hRectangle(x1, y1, x2, y2));
				continue;
			}

			if (fin.eof())
			{
				break;
			}

			fin.clear();
			fin >> trash;
		}
	}

	void SpriteManager::LoadAnimationRectangle(eAnimationType animationType, const char* fileName)
	{
		std::ifstream fin;
		fin.open(fileName, std::ios_base::in);

		assert(fin.is_open());

		float x1;
		float y1;
		float x2;
		float y2;

		mSpriteRectMap.emplace(animationType, std::vector<hRectangle>());
		std::string trash;

		while (true)
		{
			fin >> x1;
			fin >> y1;
			fin >> x2;
			fin >> y2;

			if (!fin.fail())
			{
				mSpriteRectMap[animationType].push_back(hRectangle(x1, y1, x2, y2));
				continue;
			}

			if (fin.eof())
			{
				break;
			}

			fin.clear();
			fin >> trash;
		}
	}
}