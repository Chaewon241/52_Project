#include "DemoApp.h"
#include "framework.h"
#include "../GameApp/D2DRenderer.h"
#include "../GameApp/GameObject.h"
#include "../GameApp/Transform.h"
#include "../GameApp/RectRenderer.h"
#include "../GameApp/EllipseRenderer.h"
#include "../GameApp/RenderComponent.h"
#include "../GameApp/SpriteRenderer.h"
#include "../GameApp/AnimationRenderer.h"
#include "../GameApp/Sprite.h"
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

    std::vector<std::vector<int>> vec;
    vec.push_back({ 3, 698, 61, 787});
    vec.push_back({ 73, 696, 130, 787 });
    vec.push_back({ 143, 695, 197, 787 });
    vec.push_back({ 279, 698, 337, 787 });
    vec.push_back({ 347, 699, 406, 787 });

    AnimationClip* runAni = new AnimationClip(L"Run", vec);

    GameObject* playerObject1 = new GameObject;
    gameObjectList.push_back(playerObject1);
    playerObject1->AddComponent<Transform>();
    playerObject1->GetComponent<Transform>()->SetLocalPosition({ 200, 500 });
    /*playerObject->AddComponent<SpriteRenderer>();
    playerObject->GetComponent<SpriteRenderer>()->SetSprite(new Sprite(L"Run", 28, 36, 103, 84));*/
    playerObject1->AddComponent<AnimationRenderer>();
    playerObject1->GetComponent<AnimationRenderer>()->SetAnimation(runAni);

    GameObject* playerObject2 = new GameObject;
    playerObject1->AddChild(playerObject2);
    gameObjectList.push_back(playerObject2);
    playerObject2->AddComponent<Transform>();
    playerObject2->GetComponent<Transform>()->SetLocalPosition({ 100, 100 });
    playerObject2->AddComponent<AnimationRenderer>();
    playerObject2->GetComponent<AnimationRenderer>()->SetAnimation(runAni);

    GameObject* sunObject = new GameObject;
    gameObjectList.push_back(sunObject);
    sunObject->AddComponent<Transform>();
    sunObject->GetComponent<Transform>()->SetLocalPosition({ 1200, 450 });
    sunObject->AddComponent<RectRenderer>();

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
