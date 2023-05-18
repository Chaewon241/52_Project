#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "WinApp.h"

#define NDEBUG

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
	catInWonderland::UnitTest::Vector2_DistanceTest();
	catInWonderland::UnitTest::hRectangle_Rotate();
	catInWonderland::UnitTest::hRectangle_GetCenter();
	catInWonderland::UnitTest::hRectangle_GetSize();
	catInWonderland::UnitTest::hRectangle_IsCollison();
	catInWonderland::UnitTest::hRectangle_IsContained();
	catInWonderland::UnitTest::hRectangle_GetBoundingRectangle();
	
#endif /* NDEBUG */

	return catInWonderland::WinApp::Run(hInstance, TEXT("CatInWonderland"), 1920, 1080);
}