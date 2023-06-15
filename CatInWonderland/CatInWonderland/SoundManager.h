#pragma once

#include <string>

#include "eSoundType.h"

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

namespace catInWonderland
{
	enum class eSoundChannel
	{
		BGM,
		Effect,
		Size
	};

	enum class eSoundList
	{
		BGM1,
		BGM2,
		BGM3,
		BGM4,
		BGM5,
		BGM6,
		Effect1,
		Effect2,
		Effect3,
		Effect4,
		Effect5,
		Effect6,
		Effect7,
		Effect8,
		Effect9,
		Effect10,
		Size
	};

	class SoundManager
	{
	public:
		static SoundManager* GetInstance();
		static void DestroyInstance();

		void LoadMusic(eSoundList _list,const char* music);

		void PlayMusic(eSoundList _list, eSoundChannel _channel, float vol);
		void StopMusic(eSoundChannel _channel);
		void SetVolume(eSoundChannel _channel, float vol);

	private:
	 	SoundManager();
		~SoundManager();

	private:
		static SoundManager* mInstance;

		FMOD::System* mSystem;
		FMOD::Channel* mChannel[static_cast<int>(eSoundChannel::Size)];
		FMOD::Sound* mSoundList[static_cast<int>(eSoundList::Size)];
	};
}