#include "pch.h"
#include "World.h"

constexpr int resreveSize = 30;

World::World()
{
	for(auto& gameObjects : m_gameObjects)
	{
		gameObjects.reserve(resreveSize);
	}
	m_objectMap.reserve(resreveSize * static_cast<int>(eObjectType::END));
}

World::~World()
{
	World::Finalize();
}

std::shared_ptr<GameObject> World::GetGameObject(std::string_view name)
{
	std::pair<eObjectType, int> pair = m_objectMap.find(std::string(name))->second;

	return m_gameObjects[static_cast<int>(pair.first)][pair.second];
}

void World::Update(float deltaTime)
{
	for (auto& gameObjectGroup : m_gameObjects)
	{
		for (const auto& gameObject : gameObjectGroup)
		{
			gameObject->Update(deltaTime);
		}
	}
}

void World::OnEnter()
{
	this->Initialize();
}

void World::OnExit()
{
	this->Finalize();
}

void World::Initialize()
{
	for (auto& gameObjectGroup : m_gameObjects)
	{
		for (const auto& gameObject : gameObjectGroup)
		{
			gameObject->Initialize();
		}
	}
}

void World::Finalize()
{
	for (auto& gameObjectGroup : m_gameObjects)
	{
		for (auto& gameObject : gameObjectGroup)
			gameObject.reset();

		gameObjectGroup.clear();
	}
}
