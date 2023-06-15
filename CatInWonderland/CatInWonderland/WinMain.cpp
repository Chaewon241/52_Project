#define WIN32_LEAN_AND_MEAN

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
#include <Windows.h>
#include "WinApp.h"

#ifndef NDEBUG
#include "UnitTest.h"
#endif /* NDEBUG */

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

#ifndef NDEBUG
	catInWonderland::UnitTest::Test();
#endif /* NDEBUG */

	int result = catInWonderland::WinApp::Run(hInstance, TEXT("CatInWonderland"), 1920, 1080);

	_CrtDumpMemoryLeaks();

	return result;
}