#pragma once
#include "Object.h"

namespace catInWonderland
{
	class Magnet : public Object
	{
	public:
		virtual void Render() {};
		virtual void Update() {};
		virtual void MagnetObject() {};	//도착지 (자석)
		virtual void MagnetObstacle() {};	//Magnet 상호작용 방해 오브젝트
		virtual void NonChangeBlock();		//Player 극을 바꾸지 않는 영역
	};
}