#include "pch.h"
#include "WorldManager.h"

WorldManager::WorldManager()
{
	m_worlds.reserve(static_cast<UINT>(eWorldType::END));
}

void WorldManager::ChangeWorld(eWorldType worldType)
{
	m_prevWorld = m_currentWorld;
	m_currentWorld = m_worlds[static_cast<int>(worldType)];

	m_currentWorld->OnEnter();
	m_prevWorld->OnExit();
}

void WorldManager::Initialize()
{
	m_currentWorld->OnEnter();
}

void WorldManager::Update(float deltaTime)
{
	m_currentWorld->Update(deltaTime);
}

void WorldManager::Finalize()
{
	for(auto& world:m_worlds)
	{
		world.reset();
	}

	while (!m_recvQueue.empty())
	{
		char* deleteQue = m_recvQueue.front().first;
		m_recvQueue.pop();
		delete[] deleteQue;
	}

	while (!m_sendQueue.empty())
	{
		char* deleteQue = m_sendQueue.front().first;
		m_sendQueue.pop();
		delete[] deleteQue;
	}
	Singleton<WorldManager>::Finalize();
}
