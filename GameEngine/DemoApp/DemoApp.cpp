#include "DemoApp.h"
#include "framework.h"
#include "../GameApp/D2DRenderer.h"
#include "../GameApp/GameObject.h"
#include "../GameApp/Transform.h"
#include "../GameApp/RectRenderer.h"
#include "../GameApp/EllipseRenderer.h"
#include "../GameApp/AnimationClip.h"

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

    ANIMATION_INFO Animation;
    m_pAnimationAsset = m_D2DRenderer.CreateSharedAnimationAsset(L"Test");
    m_pAnimationAsset->SetD2DBitmap(L"../resources/run.png");

    Animation.m_Name = L"Run";
    Animation.m_Frames.clear();
    Animation.m_Frames.push_back(FRAME_INFO(28, 36, 103, 84, 0.1f));
    Animation.m_Frames.push_back(FRAME_INFO(148, 36, 86, 84, 0.1f));
    Animation.m_Frames.push_back(FRAME_INFO(255, 34, 87, 86, 0.1f));
    Animation.m_Frames.push_back(FRAME_INFO(363, 32, 76, 88, 0.1f));
    Animation.m_Frames.push_back(FRAME_INFO(458, 31, 91, 89, 0.1f));
    Animation.m_Frames.push_back(FRAME_INFO(567, 40, 103, 80, 0.1f));
    Animation.m_Frames.push_back(FRAME_INFO(686, 32, 85, 88, 0.1f));
    Animation.m_Frames.push_back(FRAME_INFO(792, 32, 86, 88, 0.1f));
    Animation.m_Frames.push_back(FRAME_INFO(899, 31, 76, 89, 0.1f));
    Animation.m_Frames.push_back(FRAME_INFO(993, 33, 92, 87, 0.1f));
    m_pAnimationAsset->m_Animations.push_back(Animation);

    GameObject* sunObject = new GameObject;
    gameObjectList.push_back(sunObject);
    sunObject->AddComponent<Transform>();
    sunObject->AddComponent<RectRenderer>();
    sunObject->GetComponent<Transform>()->SetLocalPosition({ 1200, 450 });

    GameObject* earthObject = new GameObject;
    sunObject->AddChild(earthObject);
    gameObjectList.push_back(earthObject);
    earthObject->AddComponent<Transform>();
    earthObject->AddComponent<EllipseRenderer>();
    earthObject->GetComponent<Transform>()->SetLocalPosition({ 200, 200 });
    earthObject->GetComponent<Transform>()->SetLocalScale({ 0.8f, 0.8f });

    GameObject* moonObject = new GameObject;
    earthObject->AddChild(moonObject);
    gameObjectList.push_back(moonObject);
    moonObject->AddComponent<Transform>();
    moonObject->AddComponent<RectRenderer>();
    moonObject->GetComponent<Transform>()->SetLocalPosition({ 100, 100 });
    moonObject->GetComponent<Transform>()->SetLocalScale({ 0.5f, 0.5f });

    return true;
}

void DemoApp::Update()
{
    __super::Update();

    for (GameObject* go : gameObjectList)
    {
        go->Update();
    }

}

void DemoApp::Render()
{
    D2DRenderer::m_hInstance->BeginRender();
    for (GameObject* go : gameObjectList)
    {
        go->Render(D2DRenderer::m_hInstance);
    }
    D2DRenderer::m_hInstance->EndRender();
}
