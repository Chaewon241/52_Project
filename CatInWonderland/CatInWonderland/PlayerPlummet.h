#pragma once

#include <vector>

#include "PlayerState.h"
#include "hRectangle.h"

namespace catInWonderland
{
	class Player;

	class PlayerPlummet : public PlayerState
	{
	public:
		PlayerPlummet();
		~PlayerPlummet() = default;

		virtual PlayerState* HandleStateOrNull(Player* player);
		virtual void Update(Player* outPlayer);

	private:
		float mDistance;
		bool mbLeft;
		float mElapsed;

		const float mPlummetIntervalTime; // 한 프레임이 경과될 시간
		float mPlummetElapsedTime; // 요거는 총 경과시간
		unsigned int mPlummetmationIndex;	//애니메이션 값 + 1
		const std::vector<hRectangle>& mAnimationRectangles;

	};
}