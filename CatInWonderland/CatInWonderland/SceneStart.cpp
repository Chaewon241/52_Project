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
		// ��������� �����Ѵ�.
		// ��׶��带 �����Ѵ�.
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
		// ��������� ����.
	}
}