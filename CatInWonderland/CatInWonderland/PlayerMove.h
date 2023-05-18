#pragma once

#include <vector>

#include "PlayerState.h"
#include "hRectangle.h"

namespace catInWonderland
{
	class Player;

	class PlayerMove : public PlayerState
	{
	public:
		PlayerMove();
		~PlayerMove() = default;

		virtual PlayerState* HandleStateOrNull(Player* player);
		virtual void Update(Player* outPlayer);
		
	private:
		float mDistance;
		float mElapsed;

		const float mAniIntervalTime; // 한 프레임이 경과될 시간
		float mAniElapsedTime; // 요거는 총 경과시간
		unsigned int mAnimationIndex;
		const std::vector<hRectangle>& mAnimationRectangles;
	};
}