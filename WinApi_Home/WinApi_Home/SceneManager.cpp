#include "SceneManager.h"
#include "WinApp.h"

namespace catInWonderland
{
	SceneManager* SceneManager::instance = nullptr;

	SceneManager::SceneManager()
		: m_arrScene{}
		, m_pCurScene(nullptr)
	{
		// 씬 생성 이렇게? 이른 로딩?
		//m_arrScene[(UINT)SCENE_TYPE::SCENE] = new ;
		//m_arrScene[(UINT)SCENE_TYPE::TITLESCENE] = new ;
		//m_arrScene[(UINT)SCENE_TYPE::ENDINGSCENE] = new ;
	}

	SceneManager::~SceneManager() 
	{
		for (unsigned int i = 0; i < (unsigned int)SCENE_TYPE::END; ++i)
		{
			if (m_arrScene[i] != nullptr)
			{
				delete m_arrScene[i];
			}
		}
	}

	SceneManager* SceneManager::GetInstance()
	{
		if (instance == nullptr)
		{
			instance = new SceneManager();
		}
		return instance;
	}

	void SceneManager::DestroyInstance()
	{
		if (instance != nullptr)
		{
			delete instance;
			instance = nullptr;
		}
	}

	void SceneManager::Init()
	{
		// 씬 생성 이렇게? 2
		//m_arrScene[(UINT)SCENE_TYPE::SCENE] = new ;

		// 현재 씬 지정
		m_pCurScene = m_arrScene[(UINT)SCENE_TYPE::TITLESCENE];
	}
}