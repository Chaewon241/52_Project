#pragma once
#include "Object.h"

namespace catInWonderland
{
	class BoardObject : public Object
	{
	public:
		BoardObject(hRectangle worldRectangle, hRectangle spriteRectangle, Sprite* sprite, size_t boardX, size_t boardY);
		virtual ~BoardObject() = default;

		virtual void Update() = 0;

		// ���忡 ��ġ�� ������Ʈ���� ����� ����δ� ����

		virtual void RotateBoardIndex(bool bLeft);
		void Rotate(int originX, int originY, float radian);
		void AlignWorldRectagle();

		inline void BoardMove(int boardX, int boardY);

		inline size_t GetBoardX() const;
		inline size_t GetBoardY() const;

		// ���� �����ڴ� Ŭ���� ���� ���� ������ �ǹ��Ѵ�.
		// public : � Ŭ������ ���� ����
		// protected : ��Ӱ��迡�� ���� ����
		// private : ���� ���� ����

	protected:
		size_t mBoardX;
		size_t mBoardY;
	};

	void BoardObject::BoardMove(int boardX, int boardY)
	{
		mBoardX += boardX;
		mBoardY += boardY;
	}

	size_t BoardObject::GetBoardX() const
	{
		return mBoardX;
	}

	size_t BoardObject::GetBoardY() const
	{
		return mBoardY;
	}
}