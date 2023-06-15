#pragma once
#include "BoardObject.h"
#include "eMagent.h"

namespace catInWonderland
{
	class Butterfly : public BoardObject
	{
	public:
		Butterfly();
		~Butterfly() = default;

		virtual void Update();

	private:
		eMagent mMagentState;
	};
}