#include "SoundManager.h"

namespace catInWonderland
{
	SoundManager* SoundManager::mInstance = nullptr;

	SoundManager* SoundManager::GetInstance()
	{
		if (mInstance == nullptr)
		{
			mInstance = new SoundManager();
		}

		return mInstance;
	}

	void SoundManager::DestroyInstance()
	{
		delete mInstance;
		mInstance = nullptr;
	}
};
