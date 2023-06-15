#include "PlayerAction.h"
#include "PlayerIdle.h"
#include "Player.h"
#include "Brick.h"
#include "SpriteManager.h"
#include "SceneManager.h"
#include "Scene.h"


namespace catInWonderland
{
	PlayerAction::PlayerAction()
		: PlayerState(ePlayerState::Action)
		, mActionCheck(false)
	{
	}

	PlayerState* PlayerAction::HandleStateOrNull(Player* player)
	{
		if (!mActionCheck)
		{
			player->SetSprite(&SpriteManager::GetInstance()->GetSprite(eSpriteType::PlayerIdle));
			player->SetSpriteRectangle(SpriteManager::GetInstance()->GetSpriteRectangle(eAnimationType::PlayerIdle, false));
			
			return new PlayerIdle;
		}

		return nullptr;
	}

	void PlayerAction::Update(Player* player)
	{
		const std::vector<Object*>& objects = SceneManager::GetInstance()->GetCurrentScene().GetObjects();

		if (!mActionCheck)
		{
			for (auto iter = objects.begin(); iter != objects.end(); iter++)
			{
				Object* obj = (*iter);

				if (obj->GetObjectType() == eObjectType::Brick)
				{
					Brick* brick = static_cast<Brick*>(obj);	//Çü º¯È¯
					if (brick->GetBoardX() == player->GetBoardX())
					{
						brick->Action();
					}
				}
			}
			// mActionCheck = true;
		}
	}
}
