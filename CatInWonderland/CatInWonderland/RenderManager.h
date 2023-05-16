#pragma once

namespace catInWonderland
{
	// ���� �� �� �����ϱ�
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