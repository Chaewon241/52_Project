#pragma once
// ���� ������ ������ �޽��� ó��
#include <SDKDDKVer.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

class WinApp
{
public:
	// �����ڿ� �Ҹ���
	// ��״� return ���� ����
	WinApp() = default;
	~WinApp() = default;

	// �ʱ�ȭ(�ν��Ͻ��� �ü������ ������)
	void Initialize(HINSTANCE hInstance);

	// ���� �� ��
	void Run();

	// �������� ���� ��
	void Finalize();

	// ������ ���� �ʰ� �ν��Ͻ� ȣ��
	HINSTANCE GetInstance() const { return m_hInstance; }

	// const �ٿ��� �� �� �ٲ��
	HWND GetWindow() const { return m_hWnd; }

	const int GetWidth() const { return width; }
	const int GetHeight() const { return height; }

	// ������ ���ν����� ����� �� �Լ��� �����͸� ����ϴµ� ���� ��ü�� ���� ������
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