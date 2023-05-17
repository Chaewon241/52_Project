#pragma once
#include "Object.h"

namespace catInWonderland
{
	class BoardObject : public Object
	{
	public:
		BoardObject(hRectangle worldRectangle, hRectangle spriteRectangle, Sprite* sprite, size_t boardX, size_t boardY);
		~BoardObject() = default;

		virtual void Update() = 0;

		// ���忡 ��ġ�� ������Ʈ���� ����� ����δ� ����

		void Rotate(int originX, int originY, float radian);
		void RotateBoardIndex(bool bLeft);
		void AlignWorldRectagle();

		// ���� �����ڴ� Ŭ���� ���� ���� ������ �ǹ��Ѵ�.
		// public : � Ŭ������ ���� ����
		// protected : ��Ӱ��迡�� ���� ����
		// private : ���� ���� ����

	protected:
		size_t mBoardX;
		size_t mBoardY;
	};
}