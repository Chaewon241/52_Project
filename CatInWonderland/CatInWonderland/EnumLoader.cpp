#include "EnumLoader.h"
#include "SpriteManager.h"
#include "eSpriteType.h"

namespace catInWonderland
{
	void EnumLoader::LoadSprtie()
	{
		SpriteManager::GetInstance()->LoadSpriteImage(eSpriteType::Player, L"D:\\1Q_Proejct\\이상한 나라의 고양이\\4_Resources\\cat_sample.bmp");
		SpriteManager::GetInstance()->LoadSpriteImage(eSpriteType::Tile, L"D:\\1Q_Proejct\\이상한 나라의 고양이\\4_Resources\\blue_brick.bmp");
	}
}