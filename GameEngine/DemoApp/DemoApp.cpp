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

    ANIMATION_INFO Animation;

    Animation.m_Name = L"Idle";
    Animation.m_Frames.clear();
    Animation.m_Frames.push_back(FRAME_INFO(3, 698, 61, 787));
    Animation.m_Frames.push_back(FRAME_INFO(73, 696, 130, 787));
    Animation.m_Frames.push_back(FRAME_INFO(143, 695, 197, 787));
    Animation.m_Frames.push_back(FRAME_INFO(279, 698, 337, 787));
    Animation.m_Frames.push_back(FRAME_INFO(347, 699, 406, 787));
    m_pAnimationClip = new AnimationClip();
    m_pAnimationClip->m_Animations.push_back(Animation);

    Animation.m_Name = L"Move";
    Animation.m_Frames.clear();
    Animation.m_Frames.push_back(FRAME_INFO(9, 883, 61, 965));
    Animation.m_Frames.push_back(FRAME_INFO(71, 878, 130, 965));
    Animation.m_Frames.push_back(FRAME_INFO(141, 877, 204, 966));
    Animation.m_Frames.push_back(FRAME_INFO(216, 876, 278, 964));
    Animation.m_Frames.push_back(FRAME_INFO(358, 878, 407, 966));
    m_pAnimationClip->m_Animations.push_back(Animation);

    Animation.m_Name = L"Attack";
    Animation.m_Frames.clear();
    Animation.m_Frames.push_back(FRAME_INFO(731, 1685, 805, 1775));
    Animation.m_Frames.push_back(FRAME_INFO(809, 1685, 890, 1772));
    Animation.m_Frames.push_back(FRAME_INFO(896, 1679, 955, 1773));
    Animation.m_Frames.push_back(FRAME_INFO(966, 1674, 1079, 1772));
    Animation.m_Frames.push_back(FRAME_INFO(1101, 1679, 1201, 1772));
    Animation.m_Frames.push_back(FRAME_INFO(1223, 1677, 1295, 1771));
    Animation.m_Frames.push_back(FRAME_INFO(1302, 1677, 1373, 1771));
    Animation.m_Frames.push_back(FRAME_INFO(1400, 1680, 1473, 1777));
    m_pAnimationClip->m_Animations.push_back(Animation);

    GameObject* playerObject1 = new GameObject;
    gameObjectList.push_back(playerObject1);
    playerObject1->AddComponent<Transform>();
    playerObject1->GetComponent<Transform>()->SetLocalPosition({ 300, 300 });
    playerObject1->AddComponent<AnimationRenderer>();
    playerObject1->GetComponent<AnimationRenderer>()->SetAnimation(m_pAnimationClip);
    playerObject1->GetComponent<AnimationRenderer>()->SetAnimationPath(L"Ken");
    playerObject1->GetComponent<AnimationRenderer>()->SetAnimationState(L"Idle");

    GameObject* playerObject2 = new GameObject;
    playerObject1->AddChild(playerObject2);
    gameObjectList.push_back(playerObject2);
    playerObject2->AddComponent<Transform>();
    playerObject2->GetComponent<Transform>()->SetLocalPosition({ 100, 100 });
    playerObject2->GetComponent<Transform>()->SetLocalScale({ 0.8f, 0.8f });
    playerObject2->AddComponent<AnimationRenderer>();
    playerObject2->GetComponent<AnimationRenderer>()->SetAnimation(m_pAnimationClip);
    playerObject2->GetComponent<AnimationRenderer>()->SetAnimationPath(L"Ken");
    playerObject2->GetComponent<AnimationRenderer>()->SetAnimationState(L"Attack");

    GameObject* playerObject3 = new GameObject;
    playerObject2->AddChild(playerObject3);
    gameObjectList.push_back(playerObject3);
    playerObject3->AddComponent<Transform>();
    playerObject3->GetComponent<Transform>()->SetLocalPosition({ 100, 100 });
    playerObject3->GetComponent<Transform>()->SetLocalScale({ 0.8f, 0.8f });
    playerObject3->AddComponent<AnimationRenderer>();
    playerObject3->GetComponent<AnimationRenderer>()->SetAnimation(m_pAnimationClip);
    playerObject3->GetComponent<AnimationRenderer>()->SetAnimationPath(L"Ken");
    playerObject3->GetComponent<AnimationRenderer>()->SetAnimationState(L"Move");

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
