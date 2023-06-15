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

		const float mPlummetIntervalTime; // �� �������� ����� �ð�
		float mPlummetElapsedTime; // ��Ŵ� �� ����ð�
		unsigned int mPlummetmationIndex;	//�ִϸ��̼� �� + 1
		const std::vector<hRectangle>& mAnimationRectangles;

	};
}