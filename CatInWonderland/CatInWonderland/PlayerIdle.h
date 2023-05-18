#pragma once

#include <vector>

#include "PlayerState.h"
#include "hRectangle.h"

namespace catInWonderland
{
	class Player;

	class PlayerIdle : public PlayerState
	{
	public:
		PlayerIdle();
		~PlayerIdle() = default;

		virtual PlayerState* HandleStateOrNull(Player* player);
		virtual void Update(Player* player);

	private:
		const float mIntervalTime; // 한 프레임이 경과될 시간
		float mElapsedTime; // 요거는 총 경과시간
		unsigned int mAnimationIndex;
		const std::vector<hRectangle>& mAnimationRectangles;
	};
}
