#pragma once
#include "Object.h"

namespace catInWonderland
{
	enum class eMagent
	{
		Red,
		Bule
	};

	class Butterfly : public Object
	{
	public:
		virtual void Render() {};
		virtual void Update();
		virtual void Magent() {};
		virtual void Move() {};

		void Red();
		void Blue();
	private:
		size_t mBoardX;
		size_t mBoardY;
		size_t mMagentState;
	};
}