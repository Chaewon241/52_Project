#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "WinApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	return catInWonderland::WinApp::Run(hInstance, TEXT("CatInWonderland"), 1920, 1080);
}