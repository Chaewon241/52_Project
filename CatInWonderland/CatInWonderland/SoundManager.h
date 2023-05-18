#pragma once

#include <string>

// FMOD
#ifndef _WIN64
#include "../inc/fmod.hpp"
#pragma comment (lib, "lib/x86/fmod_vc.lib")
using namespace FMOD;
#endif
#ifdef _WIN64
#include "inc/fmod.hpp"
#pragma comment (lib, "lib/x64/fmod_vc.lib")
using namespace FMOD;
#endif

enum SOUND_CHANNEL
{
	SOUND_CHANNEL_BGM,
	SOUND_CHANNEL_EFFECT,
	SOUND_CHANNEL_END
};

enum SOUND_LIST
{
	SOUND_LIST_BGM1,
	SOUND_LIST_BGM2,
	SOUND_LIST_BGM3,
	SOUND_LIST_BGM4,
	SOUND_LIST_BGM5,
	SOUND_LIST_BGM6,
	SOUND_LIST_BGMEND,

	SOUND_LIST_EFFECT1,
	SOUND_LIST_EFFECT2,
	SOUND_LIST_EFFECT3,
	SOUND_LIST_EFFECT4,
	SOUND_LIST_EFFECT5,
	SOUND_LIST_EFFECT6,
	SOUND_LIST_EFFECT7,
	SOUND_LIST_EFFECT8,
	SOUND_LIST_EFFECT9,
	SOUND_LIST_EFFECT10,
	SOUND_LIST_EFFECTEND,

	SOUND_LIST_END
};

namespace catInWonderland
{
	class SoundManager
	{
	public:
		static SoundManager* GetInstance();
		static void DestroyInstance();

		void Init();

		void PlayMusic(SOUND_LIST _list, SOUND_CHANNEL _channel, float vol);
		// void StopMusic(SOUND_CHANNEL _channel);
		// void SetVolume(SOUND_CHANNEL _channel, float vol);

	private:
	 	SoundManager();
		~SoundManager();

	private:
		static SoundManager* mInstance;

		FMOD::System* mSystem;
		FMOD::Channel* mChannel[SOUND_CHANNEL::SOUND_CHANNEL_END];
		FMOD::Sound* mSoundList[SOUND_LIST::SOUND_LIST_END];
	};
}