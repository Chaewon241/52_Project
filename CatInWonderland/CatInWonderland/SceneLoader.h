#pragma once

namespace catInWonderland
{
	class SceneLoader
	{
	public:
		static void Load();
		static void LoadScene();
		static void RegisterScene();

	public:
		SceneLoader() = delete;
		~SceneLoader() = delete;
	};
}

