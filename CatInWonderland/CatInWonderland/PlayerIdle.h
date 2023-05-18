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
		const float mIntervalTime; // �� �������� ����� �ð�
		float mElapsedTime; // ��Ŵ� �� ����ð�
		unsigned int mAnimationIndex;
		const std::vector<hRectangle>& mAnimationRectangles;
	};
}
