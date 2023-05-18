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

		void MagnetObject();	//������ (�ڼ�)
		void MagnetObstacle() {};	//Magnet ��ȣ�ۿ� ���� ������Ʈ
		void NonChangeBlock() {};		//Player ���� �ٲ��� �ʴ� ����

	private:
		//Player�� Y ��ǥ�� ���� ��ġ�� ���� �� �����ܾ� �� = ������Ʈ���� Ư�� ���� �־�� ��? = Ư�� ��ǥ�� ������ ����
		hRectangle mMagneObject;

	};
}