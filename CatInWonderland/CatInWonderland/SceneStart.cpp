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
		float x1 = WinApp::GetWidth() / 3; //- WinApp::GetWidth() / 8;
		float y1 = WinApp::GetHeight() / 3;// -WinApp::GetHeight() / 8;
		float x2 = x1 + WinApp::GetWidth() / 4;
		float y2 = y1 + WinApp::GetHeight() / 4;
		mStartButton = new Button(hRectangle(x1,y1,x2,y2)
			, hRectangle(0, 0, 700, 500)
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