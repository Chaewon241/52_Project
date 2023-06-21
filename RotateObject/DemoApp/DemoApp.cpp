#include "../RotateObject/pch.h"
#include "framework.h"
#include "DemoApp.h"

int APIENTRY wWinMain
(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR lpCmdLine,
	_In_ int nCmdShow
)
{
    //DemoApp demo(hInstance, 500, 500);
}

DemoApp::DemoApp(HINSTANCE hInstance, UINT width, UINT height) : GameApp::GameApp(hInstance, 500, 500)
{
    //Show();
}

DemoApp::~DemoApp()
{
}
