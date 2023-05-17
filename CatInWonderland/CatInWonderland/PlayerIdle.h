#pragma once

#include "PlayerState.h"

class PlayerIdle : public PlayerState
{
	class Player;
public:
	PlayerState* HandleState(const Player& player) {};
	void Update(Player* player) {};
};