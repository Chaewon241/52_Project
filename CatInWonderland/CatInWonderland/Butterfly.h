#pragma once
#include "BoardObject.h"

namespace catInWonderland
{
	enum class eMagent
	{
		Red,
		Bule
	};

	class Butterfly : public BoardObject
	{
	public:
		virtual void Update();
		void Magent() {};
		void Move() {};

		void Red() {};
		void Blue() {};

	private:
		size_t mBoardX;
		size_t mBoardY;
		size_t mMagentState;
	};
}