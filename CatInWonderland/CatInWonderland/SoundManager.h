#pragma once

namespace catInWonderland
{
	class SoundManager
	{
	public:
		static SoundManager* GetInstance();
		static void DestroyInstance();

	private:
		SoundManager() = default;
		~SoundManager() = default;

	private:
		static SoundManager* mInstance;
	};
}