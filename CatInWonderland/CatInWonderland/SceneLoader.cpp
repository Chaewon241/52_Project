#include "SceneLoader.h"

#include "SceneManager.h"
#include "SceneStart.h"
#include "Stage01.h"

namespace catInWonderland
{
	void SceneLoader::Load()
	{
		LoadScene();
		RegisterScene();
	}

	void SceneLoader::LoadScene()
	{

	}

	void SceneLoader::RegisterScene()
	{
		SceneManager::GetInstance()->RegisterScene(eSceneType::START, new SceneStart());
		SceneManager::GetInstance()->RegisterScene(eSceneType::STAGE01, new Stage01());
		/*SceneManager::GetInstance()->RegisterScene(eSceneType::START, new SceneStart());
		SceneManager::GetInstance()->RegisterScene(eSceneType::START, new SceneStart());
		SceneManager::GetInstance()->RegisterScene(eSceneType::START, new SceneStart());*/

		SceneManager::GetInstance()->setCurScene(eSceneType::START);
	}
}