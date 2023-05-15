#pragma once

namespace catInWonderland
{
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

		void CreatePlayer(int x, int y, int radius);

		void EndDraw();

		void ReleaseRender();

	private:
		static RenderManager* instance;
	};

	
}