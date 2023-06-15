// DemoApp.cpp : 애플리케이션에 대한 진입점을 정의합니다.
//

#include "framework.h"
#include "DemoApp.h"
#include "../D2DRenderer/D2DRenderer.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.

    // 전역 문자열을 초기화합니다.
    DemoApp App(hInstance);
    App.Initialize();
    App.Loop();
    return (int)1;
}


DemoApp::DemoApp(HINSTANCE hInstance)
    :GameApp::GameApp(hInstance)
{
    // DemoApp에서 리소스를 얻어와 클래스 설정하기
    LoadStringW(hInstance, IDS_APP_TITLE, m_szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DEMOAPP, m_szWindowClass, MAX_LOADSTRING);

    m_wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DEMOAPP));
    m_wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    //m_wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DEMOAPP);
    m_wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL));
    m_hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DEMOAPP));
}

DemoApp::~DemoApp()
{
}

void DemoApp::Update()
{
    GameApp::Update();

}

void DemoApp::Render()
{
    D2DRenderer::m_pD2DRenderTarget->BeginDraw();
    D2D1::ColorF color(D2D1::ColorF::Red);

    D2DRenderer::m_pD2DRenderTarget->Clear(color);

    D2DRenderer::m_pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(0, 0));
    D2DRenderer::m_pD2DRenderTarget->DrawBitmap(m_pBitmap1);

    D2DRenderer::m_pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(0, 400));
    D2DRenderer::m_pD2DRenderTarget->DrawBitmap(m_pBitmap2);

    D2DRenderer::m_pD2DRenderTarget->EndDraw();
}

bool DemoApp::Initialize()
{
    GameApp::Initialize();

    D2DRenderer::m_Instance->CreateD2DBitmapFromFile(L"../resources/run.png", &m_pBitmap1);
    D2DRenderer::m_Instance->CreateD2DBitmapFromFile(L"../resources/run.png", &m_pBitmap2);

    return false;
}
