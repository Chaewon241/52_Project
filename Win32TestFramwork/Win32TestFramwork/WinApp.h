#pragma once
// 메인 윈도우 생성과 메시지 처리
#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class WinApp
{
public:
	// 생성자와 소멸자
	// 얘네는 return 값이 없음
	WinApp() = default;
	~WinApp() = default;

	// 초기화(인스턴스는 운영체제에서 가져옴)
	void Initialize(HINSTANCE hInstance);

	// 루프 들어갈 때
	void Run();

	// 루프에서 나올 때
	void Finalize();

	// 전역을 쓰지 않고 인스턴스 호출
	HINSTANCE GetInstance() const { return m_hInstance; }

	// const 붙여서 값 안 바뀌게
	HWND GetWindow() const { return m_hWnd; }

	const int GetWidth() const { return width; }
	const int GetHeight() const { return height; }

	// 윈도우 프로시저를 등록할 때 함수의 포인터를 등록하는데 아직 객체가 없기 때문에
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:

	HINSTANCE m_hInstance = { 0 };  // HINSTANCE is a handle to an instance of a module.
	HWND m_hWnd = { 0 };			// HWND is a handle to a window.

	int width = 1024;
	int height = 768;
};

namespace global
{
	const WinApp& GetWinApp();
}