#include "pch.h"
#include "TestWorld.h"

#include "../Engine/EventManager.h"
#include "../Engine/GameObject.h"
#include "../Engine/CameraComponent.h"
#include "../Engine/ControllerComponent.h"
#include "../Engine/FPSCameraController.h"
#include "../Engine/Model.h"
#include "../Engine/WorldManager.h"
#include "../NetworkLibrary/MyProtocol.h"

#include <memory>
#include <string>

#include "../Engine/StaticMeshComponent.h"
#include "../Engine/SkeletalMeshComponent.h"

void TestWorld::Initialize()
{
	//m_Camera = CreateGameObject<GameObject>("Test_Camera", eObjectType::CAMERA).lock();
	//auto cameraComponent = m_Camera->CreateComponent<CameraComponent>("Test_Camera_Component");
	//m_Camera->SetRootComponent(cameraComponent.lock());
	//auto controllerComponent = m_Camera->CreateComponent<ControllerComponent>("Test_Controller_Component");
	//controllerComponent.lock()->CreateController<FPSCameraController>();

	//std::shared_ptr<GameObject> staticMesh = CreateGameObject<GameObject>("StaticMesh", eObjectType::PLAYER).lock();
	//auto staticMeshComponent = staticMesh->CreateComponent<StaticMeshComponent>("StaticMeshComponent").lock();
	//staticMeshComponent->SetFilePath("../Resources/FBX/zeldaPosed001.fbx");
	//staticMesh->SetRootComponent(staticMeshComponent);

	//std::shared_ptr<GameObject> skeletalMesh = CreateGameObject<GameObject>("SkeletalMesh", eObjectType::PLAYER).lock();
	//auto skeletalMeshComponent = skeletalMesh->CreateComponent<SkeletalMeshComponent>("StaticMeshComponent").lock();
	//skeletalMeshComponent->SetFilePath("../Resources/FBX/Zombie_Run.fbx");
	//skeletalMeshComponent->SetLocalPosition({ 100.f, 0.f, 0.f });
	//skeletalMesh->SetRootComponent(skeletalMeshComponent);

	//CommonApp::m_pInstance->GetRenderer()->SetProjectionMatrix(cameraComponent.lock()->GetProjectionMatrix());

	LOG_CONTENTS("TestWorld1 Init");

	__super::Initialize();
}

void TestWorld::Update(float deltaTime)
{
	World::Update(deltaTime);

	if(GetAsyncKeyState(VK_RIGHT) & 0x8000)
		EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::TEST2);

	// todo 채원: Test -> TestWorld에서 delete키를 눌렀을 때 씬 바뀌게 하는거
	if (GetAsyncKeyState(VK_DELETE) & 0x8000)
	{
		// 이미 레디 상태면
		if (!m_readyState)
		{
			char* ready = new char[sizeof(PacketC2S_READY) + 1];
			ready[0] = sizeof(PacketC2S_READY) / 10 + '0';
			ready[1] = sizeof(PacketC2S_READY) % 10 + '0';
			ready[2] = C2S_READY / 10 + '0';
			ready[3] = C2S_READY % 10 + '0';
			ready[4] = '0';	// ready
			ready[5] = '1';

			WorldManager::GetInstance()->PushSendQueue(ready, sizeof(PacketC2S_READY));
		}
		else if (m_readyState)
		{
			char* ready = new char[sizeof(PacketC2S_READY) + 1];
			ready[0] = sizeof(PacketC2S_READY) / 10 + '0';
			ready[1] = sizeof(PacketC2S_READY) % 10 + '0';
			ready[2] = C2S_READY / 10 + '0';
			ready[3] = C2S_READY % 10 + '0';
			ready[4] = '0';	// ready// not ready
			ready[5] = '0';

			WorldManager::GetInstance()->PushSendQueue(ready, sizeof(PacketC2S_READY));
		}
	}

	// recv된 메시지큐에서 Start해도 된다는 패킷이 있다면 씬 바꿔주기
	if (WorldManager::GetInstance()->IsRecvQueueEmpty())
		return;

	int id = static_cast<int>((WorldManager::GetInstance()->GetRecvQueueFrontBuffer()[2] - '0') * 10 + (WorldManager::GetInstance()->GetRecvQueueFrontBuffer()[3] - '0'));
	if (id == S2C_START)
	{
		char* tmp = WorldManager::GetInstance()->GetRecvQueueFrontBuffer();
		WorldManager::GetInstance()->PopRecvQueue();
		delete[] tmp;
		EventManager::GetInstance()->SendEvent(eEventType::CHANGE_WORLD, this, eWorldType::TEST2);
	}
}

void TestWorld::OnEnter()
{
	World::OnEnter();
}

void TestWorld::OnExit()
{
	World::OnExit();
}

void TestWorld::HandleEvent(Event* event)
{
	if(event->eventID == eEventType::CHANGE_WORLD)
	{
		WorldManager::GetInstance()->ChangeWorld(event->GetData<eWorldType>());
	}
}
