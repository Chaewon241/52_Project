#include "pch.h"
#include "GameApp.h"

#include <directxtk/SimpleMath.h>

#include "TestWorld.h"

#include "../Engine/Model.h"
#include "../Engine/GameObject.h"
#include "../Engine/InputManager.h"
#include "../Engine/EventManager.h"
#include "../Engine/NetworkManager.h"

#include <vld.h>

#include "TestWorld2.h"

#pragma comment (lib, "d3d11.lib")
#ifdef ENGINE_DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif

using namespace DirectX;
using namespace DirectX::SimpleMath;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// ���� ���ڿ��� �ʱ�ȭ�մϴ�.
	GameApp App(hInstance);
	App.Initialize();
	App.Run();
	App.Finalize();

	return (int)1;
}

GameApp::GameApp(HINSTANCE hInstance)
	: CommonApp(hInstance)
{
}

bool GameApp::Initialize()
{
	m_server.Start();
	m_client.Start();

	bool res = __super::Initialize();

	std::shared_ptr<TestWorld> testWorld = WorldManager::GetInstance()->RegisterWorld<TestWorld>("testWorld", eWorldType::TEST).lock();
	WorldManager::GetInstance()->SetDefaultWorld(testWorld);
	EventManager::GetInstance()->RegisterListener(eEventType::CHANGE_WORLD, testWorld.get());

	std::shared_ptr<TestWorld2> testWorld2 = WorldManager::GetInstance()->RegisterWorld<TestWorld2>("testWorld2", eWorldType::TEST2).lock();
	EventManager::GetInstance()->RegisterListener(eEventType::CHANGE_WORLD, testWorld2.get());

	WorldManager::GetInstance()->Initialize();


	/// �̺�Ʈ �Ŵ����� ���� ����
	EventManager::GetInstance()->Initialize();

	return res;
}

void GameApp::Update()
{
	// Todo: ä�� : Host�� ���� ���� ������Ʈ �� ���൵ ��.
	m_server.Update();
	m_server.NetUpdate();
	m_client.Update();
	m_client.NetUpdate();

	CommonApp::Update();

	WorldManager::GetInstance()->Update(m_deltaTime);

	/// �̺�Ʈ �Ŵ����� ���� ����
	EventManager::GetInstance()->Update();
}

void GameApp::Render()
{
	__super::Render();
}

void GameApp::Finalize()
{
	// Todo: ä�� : Test
	m_server.Stop();
	m_client.Stop();
	WorldManager::GetInstance()->Finalize();
	EventManager::GetInstance()->Finalize();
	CommonApp::Finalize();
}

LRESULT GameApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return __super::WndProc(hWnd, message, wParam, lParam);
}
