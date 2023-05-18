#pragma once

#include "BoardObject.h"
#include "ePlayerState.h"
#include "PlayerState.h"

namespace catInWonderland
{
	class PlayerState;

	class Player : public BoardObject
	{
	public:
		Player(hRectangle worldRectangle, hRectangle spriteRectangle, Sprite* sprite, size_t boardX, size_t boardY);
		virtual ~Player();

		virtual void Update();
		virtual void RotateBoardIndex(bool bLeft);

		void CheckFloor();
		void Move(float x, float y);

		inline void SetSpriteRectangle(const hRectangle& spriteRectangle);
		inline void SetIsLeft(bool bLeft);

		inline bool GetIsLeft() const; // 함수 뒤에 const는 호출 이후에 맴버 변수 수정이 없음을 의미한다.
		inline ePlayerState GetPlayerState() const;

	private:
		PlayerState* mPlayerState;
		bool mbLeft;
	};

	void Player::SetSpriteRectangle(const hRectangle& spriteRectangle)
	{
		mSpriteRectangle = spriteRectangle;
	}

	void Player::SetIsLeft(bool bLeft)
	{
		if (mbLeft != bLeft)
		{
			Vector2 temp = mWorldRectangle.GetTopLeft();
			mWorldRectangle.SetTopLeft(mWorldRectangle.GetTopRight());
			mWorldRectangle.SetTopRight(temp);

			temp = mWorldRectangle.GetBottomLeft();
			mWorldRectangle.SetBottomLeft(mWorldRectangle.GetBottomRight());
			mWorldRectangle.SetBottomRight(temp);
		}

		mbLeft = bLeft;
	}

	// inline : 컴파일러님 최적화 가능하면 해주세요
	bool Player::GetIsLeft() const
	{
		return mbLeft;
	}

	ePlayerState Player::GetPlayerState() const
	{
		return mPlayerState->GetPlayerState();
	}
}