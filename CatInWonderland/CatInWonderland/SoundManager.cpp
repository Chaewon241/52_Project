#include "SoundManager.h"
#include "WinApp.h"

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

	void SoundManager::LoadMusic(eSoundList _list, const char* music)
	{
		System_Create(&mSystem);
		mSystem->init(2, FMOD_INIT_NORMAL, 0);

		mSystem->createSound(music, FMOD_LOOP_NORMAL, 0, &mSoundList[static_cast<int>(_list)]);
	}	

	void SoundManager::PlayMusic(eSoundList _list, eSoundChannel _channel, float vol)
	{
		//mChannel[static_cast<int>(_channel)]->stop();
		mChannel[static_cast<int>(_channel)]->setVolume(vol);
		mSystem->playSound(mSoundList[static_cast<int>(_list)], NULL, 0, &mChannel[static_cast<int>(_channel)]);
	}

	void SoundManager::StopMusic(eSoundChannel _channel)
	{
		mChannel[static_cast<int>(_channel)]->stop();
	}

	void SoundManager::SetVolume(eSoundChannel _channel, float vol)
	{
		mChannel[static_cast<int>(_channel)]->setVolume(vol);
	}

	SoundManager::SoundManager()
		: mSystem()
		, mChannel{}
		, mSoundList{}
	{
	}

	SoundManager::~SoundManager()
	{
		mSystem->release();
		mSystem->close();
	}
};
