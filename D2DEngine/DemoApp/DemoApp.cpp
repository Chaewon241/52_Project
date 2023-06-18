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

    m_Background.Update(m_deltaTime);
    m_Character.Update(m_deltaTime);
}

void DemoApp::Render()
{
    D2DRenderer::m_pD2DRenderTarget->BeginDraw();
    D2DRenderer::m_pD2DRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::CadetBlue));

    /*D2DRenderer::m_pD2DRenderTarget->SetTransform(D2D1::Matrix3x2F::Translation(0, 400));
    D2DRenderer::m_pD2DRenderTarget->DrawBitmap(m_pD2DBitmap2);*/

    m_Background.Render(D2DRenderer::m_pD2DRenderTarget);
    m_Character.Render(D2DRenderer::m_pD2DRenderTarget);

    D2DRenderer::m_pD2DRenderTarget->EndDraw();
}

bool DemoApp::Initialize()
{
    GameApp::Initialize();

    AnimationAsset* m_pAnimAssetBackGround = new AnimationAsset;
    AnimationAsset* m_pAnimAssetCharacter = new AnimationAsset;

    std::vector<FRAME_INFO> Frames;

    m_pAnimAssetBackGround->SetBitmapFilePath(L"../resources/midnight.png");
    m_pAnimAssetBackGround->Build();
    Frames.push_back(FRAME_INFO(0, 0, 784, 320, 0.2f));
    Frames.push_back(FRAME_INFO(789, 0, 784, 320, 0.2f));
    Frames.push_back(FRAME_INFO(0, 325, 784, 320, 0.2f));
    Frames.push_back(FRAME_INFO(789, 325, 784, 320, 0.2f));
    m_pAnimAssetBackGround->m_Animations.push_back(Frames);

    m_Background.SetAnimationInfo(m_pAnimAssetBackGround);

    m_pAnimAssetCharacter->SetBitmapFilePath(L"../resources/run.png");
    m_pAnimAssetCharacter->Build();

    Frames.clear();

    Frames.push_back(FRAME_INFO(28, 36, 103, 84, 0.1f));
    Frames.push_back(FRAME_INFO(148, 36, 86, 84, 0.1f));
    Frames.push_back(FRAME_INFO(255, 34, 87, 86, 0.1f));
    Frames.push_back(FRAME_INFO(363, 32, 76, 88, 0.1f));
    Frames.push_back(FRAME_INFO(458, 31, 91, 89, 0.1f));
    Frames.push_back(FRAME_INFO(567, 40, 103, 80, 0.1f));
    Frames.push_back(FRAME_INFO(686, 32, 85, 88, 0.1f));
    Frames.push_back(FRAME_INFO(792, 32, 86, 88, 0.1f));
    Frames.push_back(FRAME_INFO(899, 31, 76, 89, 0.1f));
    Frames.push_back(FRAME_INFO(993, 33, 92, 87, 0.1f));
    m_pAnimAssetCharacter->m_Animations.push_back(Frames);

    m_Character.SetAnimationInfo(m_pAnimAssetCharacter);
    m_Character.SetAnimationIndex(0, false);
    m_Character.SetSpeed(2.0f);
    m_Character.SetPosition(200.0f, 200.0f);

    return true;
}
