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
		static void DestroyInstance();

		void CreateBoard(size_t boardSize, size_t boardPadding, size_t cellPadding);

		void AlignRectangle(size_t indexX, size_t indexY, hRectangle* outRectangle);
		void RotateRight(size_t* outIndexX, size_t* outIndexY) const;
		void RotateLeft(size_t* outIndexX, size_t* outIndexY) const;

		inline size_t GetCellSize() const;
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

	size_t BoardManager::GetCellSize() const
	{
		assert(!mWorldRectangles.empty());

		return mWorldRectangles.at(0).GetWidth();
	}

	size_t BoardManager::GetBoardSize() const
	{
		return mBoardSize;
	}

	const hRectangle& BoardManager::GetWorldRectangle(size_t objectX, size_t objectY) const
	{
		assert(objectX < mBoardSize || objectY < mBoardSize);

		return mWorldRectangles.at(objectX + objectY * mBoardSize);
	}
}