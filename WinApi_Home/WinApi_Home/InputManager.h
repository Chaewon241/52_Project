#pragma once

namespace catInWonderland
{
	class InputManager
	{
	public:
		InputManager();

		~InputManager();

		static InputManager* GetInstance();

		static void DestroyInstance();

		void Init();

		void ResetInput();

		void KeyDown(unsigned int key);

		void KeyUp(unsigned int key);

		bool IsKeyDown(unsigned int key);

		bool IsKeyUp(unsigned int key);

	private:
		static InputManager* instance;
	};
}