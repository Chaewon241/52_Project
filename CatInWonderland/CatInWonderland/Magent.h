#pragma once
#include "BoardObject.h"

namespace catInWonderland
{
	class Magnet : public BoardObject
	{
	public:
		Magnet(hRectangle worldRectangle, hRectangle spriteRectangle, Sprite* sprite, size_t boardX, size_t boardY, hRectangle magneObject);
		~Magnet() = default;

		virtual void Update();

		void MagnetObject();	//도착지 (자석)
		void MagnetObstacle() {};	//Magnet 상호작용 방해 오브젝트
		void NonChangeBlock() {};		//Player 극을 바꾸지 않는 영역

	private:
		//Player의 Y 좌표와 같은 위치에 있을 때 끌어당겨야 함 = 오브젝트만의 특정 값이 있어야 함? = 특정 좌표를 저장할 변수
		hRectangle mMagneObject;

	};
}