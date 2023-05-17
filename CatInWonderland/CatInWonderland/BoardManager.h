#pragma once

#include <cassert>
#include <vector>

#include "hRectangle.h"

namespace catInWonderland
{
	class BoardManager
	{
	public:
		static BoardManager* GetInstance();
		static void DeleteInstance();

		void Apply();

		void AlignRectangle(size_t indexX, size_t indexY, hRectangle* outRectangle);
		void RotateRight(size_t* outIndexX, size_t* outIndexY) const;
		void RotateLeft(size_t* outIndexX, size_t* outIndexY) const;

		inline void SetBoardSize(size_t boardSize);
		inline void SetBoardPadding(size_t boardPadding);
		inline void SetCellPadding(size_t cellPadding);

		inline size_t GetBoardSize() const;
		inline const hRectangle& GetWorldRectangle(size_t objectX, size_t ObjectY) const;

	private:
		BoardManager();
		~BoardManager() = default;

	private:
		enum { RESERVE_SIZE = 128 };

		static BoardManager* mInstance;

		size_t mBoardSize;
		size_t mBoardPadding;
		size_t mCellPadding;
		std::vector<hRectangle> mWorldRectangles;
	};

	size_t BoardManager::GetBoardSize() const
	{
		return mBoardSize;
	}

	const hRectangle& BoardManager::GetWorldRectangle(size_t objectX, size_t objectY) const
	{
		assert(objectX < mBoardSize || objectY < mBoardSize);

		return mWorldRectangles.at(objectX + objectY * mBoardSize);
	}

	void BoardManager::SetBoardSize(size_t boardSize)
	{
		mBoardSize = boardSize;
	}

	void BoardManager::SetBoardPadding(size_t boardPadding)
	{
		mBoardPadding = boardPadding;
	}

	void BoardManager::SetCellPadding(size_t cellPadding)
	{
		mCellPadding = cellPadding;
	}
}