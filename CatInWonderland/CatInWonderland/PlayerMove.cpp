#include "PlayerMove.h"
#include "Player.h"

namespace catInWonderland
{
	PlayerState* PlayerMove::HandleStateOrNull(const Player& player)
	{

		return nullptr;
	}

	void PlayerMove::Update(Player* player)
	{
		static int x = 100;
		static int y = 100;

		/*if (GetAsyncKeyState('D') & 0x8000)
		{
			pos.X += 10.f;
		}
		if (GetAsyncKeyState('A') & 0x8000)
		{
			MessageBox(hWnd, _T("A버튼 눌림"), _T("눌렸당"), MB_OK);
		*/
	}
}
