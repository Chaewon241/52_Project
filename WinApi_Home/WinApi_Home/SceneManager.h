#pragma once

#include <Windows.h>
#include "define.h"

namespace catInWonderland
{
	class Scene;

	class SceneManager
	{
	public:
		SceneManager();
		~SceneManager();

		static SceneManager* GetInstance();
		static void DestroyInstance();

		void Init();

	private:
		static SceneManager* instance;

		Scene* m_arrScene[(UINT)(SCENE_TYPE::END)];	// ¸ðµç ¾À ¸ñ·Ï
		Scene* m_pCurScene;							// ÇöÀç ¾À
	};
}
