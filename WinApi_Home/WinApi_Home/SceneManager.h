#pragma once

#include <vector>
#include <string>
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
		Scene* m_arrScene[(unsigned int)SCENE_TYPE::END];	// ��� �� ���
		Scene* m_pCurScene;									// ���� ��

		static SceneManager* instance;
	};
}
