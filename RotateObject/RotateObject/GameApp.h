#pragma once

class GameApp
{
public:
	static HINSTANCE global_instance;
	static HWND global_handle;
public:
	GameApp() {}
	GameApp(HINSTANCE hInstance, const UINT& width, const UINT& height);
	virtual ~GameApp();

	void Show();
	bool Update();

	const UINT GetWidth();
	const UINT GetHeight();
};

