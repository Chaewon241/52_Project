#include "DemoApp.h"
#include "framework.h"
#include "../GameApp/D2DRenderer.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    DemoApp App(hInstance);
    App.Initialize();
    App.Loop();

    return (int) 1;
}

DemoApp::DemoApp(HINSTANCE hInstance)
	:GameApp(hInstance)
{
    m_wcex.hIcon = LoadIcon(nullptr, IDI_ERROR);
    m_wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    m_wcex.hIconSm = LoadIcon(nullptr, IDI_INFORMATION);
}

DemoApp::~DemoApp()
{
}

bool DemoApp::Initialize() 
{
    __super::Initialize();

    return true;
}

void DemoApp::Render()
{
    D2DRenderer::m_hInstance->Render();
}
