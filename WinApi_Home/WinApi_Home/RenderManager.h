#pragma once

namespace catInWonderland
{
	// 여기 좀 더 보완하기
	class RenderManager
	{
	public:
		RenderManager();

		~RenderManager();

		static RenderManager* GetInstance();

		static void DestroyInstance();

		void Init();

		void ClearScreen();

		void BeginDraw();

		void DrawPlayer(int x, int y, int radius);

		void EndDraw();

		void ReleaseRender();

	private:
		static RenderManager* instance;
	};

	
}