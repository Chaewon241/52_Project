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

	void SoundManager::Init()
	{
		System_Create(&mSystem);
		// 이거 없애고 load 함수에 추가
		// enumclass도 하기
		mSystem->init(2, FMOD_INIT_NORMAL, 0);

		mSystem->createSound("BGM1.mp3", FMOD_LOOP_NORMAL, 0, &mSoundList[SOUND_LIST::SOUND_LIST_BGM1]);
		mSystem->createSound("BGM2.mp3", FMOD_LOOP_NORMAL, 0, &mSoundList[SOUND_LIST::SOUND_LIST_BGM2]);
		//mSystem->createSound("BGM3.mp3", FMOD_LOOP_NORMAL, 0, &mSoundList[SOUND_LIST::SOUND_LIST_BGM03]);
		//mSystem->createSound("BGM4.mp3", FMOD_LOOP_NORMAL, 0, &mSoundList[SOUND_LIST::SOUND_LIST_BGM04]);
		//mSystem->createSound("BGM5.mp3", FMOD_LOOP_NORMAL, 0, &mSoundList[SOUND_LIST::SOUND_LIST_BGM05]);
		//mSystem->createSound("BGM6.mp3", FMOD_LOOP_NORMAL, 0, &mSoundList[SOUND_LIST::SOUND_LIST_BGM06]);
		/*
		mSystem->createSound("EFFECT1.mp3", FMOD_LOOP_OFF, 0, &mSoundList[SOUND_LIST::SOUND_LIST_EFFECT01]);
		mSystem->createSound("EFFECT2.mp3", FMOD_LOOP_OFF, 0, &mSoundList[SOUND_LIST::SOUND_LIST_EFFECT02]);
		mSystem->createSound("EFFECT3.mp3", FMOD_LOOP_OFF, 0, &mSoundList[SOUND_LIST::SOUND_LIST_EFFECT03]);
		mSystem->createSound("EFFECT4.mp3", FMOD_LOOP_OFF, 0, &mSoundList[SOUND_LIST::SOUND_LIST_EFFECT04]);
		mSystem->createSound("EFFECT5.mp3", FMOD_LOOP_OFF, 0, &mSoundList[SOUND_LIST::SOUND_LIST_EFFECT05]);
		mSystem->createSound("EFFECT6.mp3", FMOD_LOOP_OFF, 0, &mSoundList[SOUND_LIST::SOUND_LIST_EFFECT06]);
		mSystem->createSound("EFFECT7.mp3", FMOD_LOOP_OFF, 0, &mSoundList[SOUND_LIST::SOUND_LIST_EFFECT07]);
		mSystem->createSound("EFFECT8.mp3", FMOD_LOOP_OFF, 0, &mSoundList[SOUND_LIST::SOUND_LIST_EFFECT08]);
		mSystem->createSound("EFFECT9.mp3", FMOD_LOOP_OFF, 0, &mSoundList[SOUND_LIST::SOUND_LIST_EFFECT09]);
		mSystem->createSound("EFFECT10.mp3", FMOD_LOOP_OFF, 0, &mSoundList[SOUND_LIST::SOUND_LIST_EFFECT10]);*/
	}


	void SoundManager::PlayMusic(SOUND_LIST _list, SOUND_CHANNEL _channel, float vol)
	{
		mChannel[_channel]->stop();
		mChannel[_channel]->setVolume(vol);
		mSystem->playSound(mSoundList[_list], NULL, 0, &mChannel[_channel]);
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
