#pragma once

#include "World.h"
#include "Singleton.h"

#include <queue>

///<summary>
///World�� ����� ����ϸ� worldManager�� ���� init, update�ȴ�.
///</summary>
class WorldManager
	: public Singleton<WorldManager>
{
public:
	WorldManager();
	~WorldManager() = default;

public:
	std::shared_ptr<World> GetCurWorld() { return m_currentWorld; }

	void SetDefaultWorld(std::shared_ptr<World> world) { m_currentWorld = world; }

	void ChangeWorld(eWorldType worldType);

	char* GetRecvQueueFrontBuffer() { return m_recvQueue.front().first; }
	char* GetSendQueueFrontBuffer() { return m_sendQueue.front().first; }

	int GetRecvQueueFrontLen() { return m_recvQueue.front().second; }
	int GetSendQueueFrontLen() { return m_sendQueue.front().second; }

	void PushRecvQueue(char* pBuf, int len) { m_recvQueue.push({ pBuf, len }); }
	void PushSendQueue(char* pBuf, int len) { m_sendQueue.push({ pBuf, len }); }

	void PopRecvQueue() { m_recvQueue.pop(); }
	void PopSendQueue() { m_sendQueue.pop(); }

	bool IsRecvQueueEmpty() { return m_recvQueue.empty(); }
	bool IsSendQueueEmpty() { return m_sendQueue.empty(); }

	void Initialize();
	void Update(float deltaTime);
	void Finalize() override;

	//TODO : (�⸲) ����Ŵ������� ���� �����ϵ��� ����, ���� ���� �̺�Ʈ ���� ����Ŵ��� �ʿ��� ó���ϵ���
private:
	std::vector<std::shared_ptr<World>> m_worlds;
	std::shared_ptr<World> m_currentWorld;
	std::shared_ptr<World> m_prevWorld;

	// ���۶� length�� ���� �ִ� ť
	std::queue<std::pair<char*, int>> m_sendQueue;
	std::queue<std::pair<char*, int>> m_recvQueue;

	int m_sendBytes = 0;
	int m_recvBytes = 0;

public:
	template <typename T>
	std::weak_ptr<T> RegisterWorld(std::string_view worldName, eWorldType worldType)
	{
		std::shared_ptr<T> world = std::make_shared<T>();

		World* inputWorld = static_cast<World*>(world.get());
		assert(inputWorld);

		world->SetName(worldName);
		world->SetWorldType(worldType);

		m_worlds.emplace_back(world);

		return world;
	}
};

