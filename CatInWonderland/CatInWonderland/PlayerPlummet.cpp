#include "PlayerPlummet.h"
#include "BoardManager.h"


namespace catInWonderland
{
    PlayerPlummet::PlayerPlummet()
        :PlayerState(ePlayerState::Plummet)
        , mDistance(BoardManager::GetInstance()->GetCellSize())
        , mbLeft(false)
        , mMoveTimeOut(1.0f)
    {

    }

    PlayerState* catInWonderland::PlayerPlummet::HandleStateOrNull(const Player& player)
    {
        return nullptr;
    }

    void PlayerPlummet::Update(Player* outPlayer)
    {

    }
}
