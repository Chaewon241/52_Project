#pragma once
#include "Object.h"

namespace catInWonderland
{
	class Magnet : public Object
	{
	public:
		virtual void Render() {};
		virtual void Update() {};
		virtual void MagnetObject() {};	//������ (�ڼ�)
		virtual void MagnetObstacle() {};	//Magnet ��ȣ�ۿ� ���� ������Ʈ
		virtual void NonChangeBlock();		//Player ���� �ٲ��� �ʴ� ����
	};
}