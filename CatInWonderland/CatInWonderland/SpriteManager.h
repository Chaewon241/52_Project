#pragma once

#include <cassert>
#include <map>
#include <vector>
#include <Windows.h>

#include "eSpriteType.h"
#include "eAnimaitonType.h"
#include "hRectangle.h"

namespace catInWonderland
{
	struct Sprite;
	class hRectangle;

	class SpriteManager
	{
	public:
		static SpriteManager* GetInstance();
		static void DestroyInstance();

		inline Sprite& GetSprite(eSpriteType spriteType);
		inline const std::vector<hRectangle>& GetSpriteRectangles(eAnimationType animationType);
		inline const hRectangle& GetSpriteRectangle(eAnimationType animationType, bool bLoop, size_t index = 0u);

		void LoadSpriteImage(eSpriteType spriteType, const WCHAR* fileName);
		void LoadSpriteImage(eSpriteType spriteType, const char* fileName);
		void LoadAnimationRectangle(eAnimationType animationType, const WCHAR* fileName);
		void LoadAnimationRectangle(eAnimationType animationType, const char* fileName);

	private:
		SpriteManager() = default;
		~SpriteManager();

	private:
		static SpriteManager* mInstance;

		std::map<eSpriteType, Sprite*> mSpriteMap;
		std::map<eAnimationType, std::vector<hRectangle>> mSpriteRectMap;
	};

	Sprite& SpriteManager::GetSprite(eSpriteType spriteType)
	{
		auto finded = mSpriteMap.find(spriteType);
		assert(finded != mSpriteMap.end());

		return *(finded->second);
	}

	const std::vector<hRectangle>& SpriteManager::GetSpriteRectangles(eAnimationType animationType)
	{
		auto finded = mSpriteRectMap.find(animationType);
		assert(finded != mSpriteRectMap.end());

		return finded->second;
	}

	const hRectangle& SpriteManager::GetSpriteRectangle(eAnimationType animationType, bool bLoop, size_t index)
	{
		auto finded = mSpriteRectMap.find(animationType);
		assert(finded != mSpriteRectMap.end());

		std::vector<hRectangle>& spriteRectangles = (finded->second);
		size_t spriteCount = spriteRectangles.size();
		size_t aniIndex;

		if (bLoop)
		{
			aniIndex = index % spriteCount;
		}
		else
		{
			aniIndex = index < spriteCount ? index : spriteCount - 1;
		}

		assert(index < spriteCount);
		return spriteRectangles.at(aniIndex);
	}
}