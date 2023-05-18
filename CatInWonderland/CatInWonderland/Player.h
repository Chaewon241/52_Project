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

		inline bool GetIsLeft() const; // �Լ� �ڿ� const�� ȣ�� ���Ŀ� �ɹ� ���� ������ ������ �ǹ��Ѵ�.
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

	// inline : �����Ϸ��� ����ȭ �����ϸ� ���ּ���
	bool Player::GetIsLeft() const
	{
		return mbLeft;
	}

	ePlayerState Player::GetPlayerState() const
	{
		return mPlayerState->GetPlayerState();
	}
}