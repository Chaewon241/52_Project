#include "SceneStart.h"
#include "Button.h"
#include "WinApp.h"
#include "SpriteManager.h"

namespace catInWonderland
{
	SceneStart::SceneStart()
		: mStartButton(nullptr)
	{

	}

	SceneStart::~SceneStart()
	{
		Exit();
	}

	void SceneStart::Enter()
	{
		float x1 = WinApp::GetWidth() / 2 - WinApp::GetWidth() / 4;
		float y1 = WinApp::GetHeight() / 2 - WinApp::GetHeight() / 4;
		float x2 = x1 + WinApp::GetWidth() / 2;
		float y2 = y1 + WinApp::GetHeight() / 2;
		mStartButton = new Button(hRectangle(x1,y1,x2,y2)
			, SpriteManager::GetInstance()->GetSpriteRectangle(eAnimationType::PlayerIdle, true)
			, &SpriteManager::GetInstance()->GetSprite(eSpriteType::PlayerIdle));

		RegisterObject(mStartButton);
	}

	eSceneType SceneStart::HandleScene()
	{
		if (mStartButton->GetButtonState() == eButtonState::Click)  
		{
			return eSceneType::STAGE01;
		}

		return mSceneType;
	}

	void SceneStart::Exit()
	{
		mStartButton = nullptr;
	}
}