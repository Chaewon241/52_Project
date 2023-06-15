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
		// 참으로 바꿔줄 수 있는 함수가 필요하고
		// 플레이어의 위치를 추적할 방법이 필요해 == 위치에 도달해야 성공한거니까
		// 플레이어도 벽돌의 추적할 수 있어야 하고,
		// 플레이어와 벽돌이 

		void Action();
		inline eMagent GetMagentState() const;

	private:
		float mDiffX;	//총 칸
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