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

		// 보드에 배치될 오브젝트들의 기능을 묶어두는 역할

		virtual void RotateBoardIndex(bool bLeft);
		void Rotate(int originX, int originY, float radian);
		void AlignWorldRectagle();

		inline void BoardMove(int boardX, int boardY);

		inline size_t GetBoardX() const;
		inline size_t GetBoardY() const;

		// 접근 제어자는 클래스 간의 접근 권한을 의미한다.
		// public : 어떤 클래스든 접근 가능
		// protected : 상속관계에서 접근 가능
		// private : 나만 접근 가능

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