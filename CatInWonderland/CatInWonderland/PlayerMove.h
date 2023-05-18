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

		const float mAniIntervalTime; // �� �������� ����� �ð�
		float mAniElapsedTime; // ��Ŵ� �� ����ð�
		unsigned int mAnimationIndex;
		const std::vector<hRectangle>& mAnimationRectangles;
	};
}