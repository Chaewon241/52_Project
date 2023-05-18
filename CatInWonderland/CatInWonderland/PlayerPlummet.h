#pragma once

#include "PlayerState.h"

namespace catInWonderland
{
	class Player;

	class PlayerPlummet : public PlayerState
	{
	public:
		PlayerPlummet();
		~PlayerPlummet() = default;

		virtual PlayerState* HandleStateOrNull(const Player& player);
		virtual void Update(Player* outPlayer);

	private:
		float mDistance;
		bool mbLeft;
		float mMoveTimeOut;
	};
}