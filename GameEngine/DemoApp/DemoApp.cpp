#include "DemoApp.h"
#include "framework.h"
#include "../GameApp/D2DRenderer.h"
#include "../GameApp/GameObject.h"
#include "../GameApp/Transform.h"
#include "../GameApp/RectRenderer.h"

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

    GameObject* sunObject = new GameObject;
    gameObjectList.push_back(sunObject);
    sunObject->AddComponent<Transform>();
    sunObject->AddComponent<RectRenderer>();
    sunObject->GetComponent<Transform>()->SetLocalPosition({ 1000, 500 });

    GameObject* earthObject = new GameObject;
    gameObjectList.push_back(earthObject);
    earthObject->AddComponent<Transform>();
    earthObject->AddComponent<RectRenderer>();
    earthObject->GetComponent<Transform>()->SetLocalPosition({ 1000, 50 });

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
