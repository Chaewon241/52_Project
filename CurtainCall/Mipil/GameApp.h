#pragma once

#include "../Engine/Client.h"
#include  "../Engine/CommonApp.h"
#include "../Engine/Server.h"
#include "../Engine/WorldManager.h"

#include <memory>

class WorldManager;
class Model;
class TestWorld;

/// <summary>
///	���� �����Ǵ� ���� ���ø����̼�
/// �Ŵ������� Init, Update, Render�Ѵ�.
/// </summary>
class GameApp
	: public CommonApp
{
	///Test
private:
	// Todo: ä�� : Test
	Server m_server;
	ClientNetworkManager m_client;

public:
	GameApp(HINSTANCE hInstance);
	~GameApp() = default;

public:
	// __suer::init ������ ���̴� ���� �̸� �����ؾ���
	bool Initialize() final;
	void Update() final;
	void Render() final;
	void Finalize() final;

private:
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
};

