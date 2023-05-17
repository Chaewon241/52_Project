#include "SceneStart.h"

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
		//mStartButton = new Button();
		// 배경음악을 실행한다.
		// 백그라운드를 변경한다.
	}

	eSceneType SceneStart::HandleScene()
	{
		// if (mStartButton->isCliked())
		// {
		// 	return eSceneType::STAGE_01;
		// }
		// 
		// return eSceneType::START;

		return mSceneType;
	}

	void SceneStart::Exit()
	{
		/*delete mStartButton;
		mStartButton = nullptr;*/
		// 배경음악을 끈다.
	}
}