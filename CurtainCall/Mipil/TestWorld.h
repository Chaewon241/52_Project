#pragma once

#include "../Engine/World.h"
#include "../Engine/EventListener.h"

class Model;

class TestWorld
	: public World
	, public EventListener
{
///TEST
private:
	std::shared_ptr<Model> m_model;
	std::shared_ptr<GameObject> m_Camera;

	bool m_readyState = false;

public:
	void Initialize() override;
	void Update(float deltaTime) override;

public:
	void OnEnter() override;
	void OnExit() override;

public:
	void HandleEvent(Event* event) override;
};

