#pragma once

#include "BoardObject.h"
#include "eMagent.h"

namespace catInWonderland
{
	class Brick : public BoardObject
	{
	public:
		Brick(hRectangle worldRectangle, hRectangle spriteRectangle, Sprite* sprite, size_t boardX, size_t boardY);
		virtual ~Brick() = default;

		virtual void Update();
		// ������ �ٲ��� �� �ִ� �Լ��� �ʿ��ϰ�
		// �÷��̾��� ��ġ�� ������ ����� �ʿ��� == ��ġ�� �����ؾ� �����ѰŴϱ�
		// �÷��̾ ������ ������ �� �־�� �ϰ�,
		// �÷��̾�� ������ 

		void Action();
		inline eMagent GetMagentState() const;

	private:
		float mDiffX;	//�� ĭ
		float mDiffY;
		float mElapsedX;
		float mElapsedY;

		bool mbAction;
		bool mbFrame;

		eMagent mMagentState;
	};

	eMagent Brick::GetMagentState() const
	{
		return mMagentState;
	}
}