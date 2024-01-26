#pragma once

#include "GameObject.h"

#include <memory>
#include <vector>
#include <unordered_map>
#include <cassert>

enum class eWorldType
{
	TEST,
	TEST2,

	END
};

class StaticMeshModel;
class SkeletalMeshModel;

/// <summary>
/// World�� ���� Ŭ����
/// GameApp�� World �Ŵ����� ���� World���� �����Ѵ�.
///	Init, Finalize�� OnEnter, OnExit�� ���еǾ� �ִ�.
///	���� ���������� world->init, finalize�� ���� �� ��
/// World�� GameObject ����Ʈ�� ��� �ִ�.
/// </summary>
class World
	: public std::enable_shared_from_this<World>
{
public:
	World();
	virtual ~World();

public:
	// GetGameObjects�� ���� ���� ���͸� ������� ������ ����Ͽ� ��ü ���� ȥ���� �ָ� �ȵȴ�.
	// ��� �뵵 ���� ���� �Ŀ� ����
	// std::vector<std::weak_ptr<GameObject>>& GetGameObjects(eObjectType type) const;
	std::shared_ptr<GameObject> GetGameObject(std::string_view name);
	std::string GetName() const { return m_name; }
	
	void SetWorldType(eWorldType worldType) { m_worldType = worldType; }
	void SetName(std::string_view name) { m_name = name; }

public:
	virtual void Update(float deltaTime);

	/// Init, Finalize �̿ܿ� ���� ��ȯ�� �ʿ��� �߰� �ൿ
	virtual void OnEnter();
	virtual void OnExit();

protected:
	/// ������Ʈ ����, �ı�
	virtual void Initialize();
	virtual void Finalize();


protected:
	std::string m_name;
	eWorldType m_worldType = eWorldType::END;

	std::vector<std::shared_ptr<GameObject>> m_gameObjects[static_cast<int>(eObjectType::END)];
	std::unordered_map<std::string, std::pair<eObjectType, int>> m_objectMap;


public:
	template <typename T>
	std::weak_ptr<T> CreateGameObject(std::string_view objName, eObjectType objectType)
	{
		std::shared_ptr<T> obj = std::make_shared<T>();
		GameObject* inputObj = static_cast<GameObject*>(obj.get());
		assert(inputObj);

		obj->SetName(objName);
		obj->SetObjectType(objectType);
		obj->SetOwnerWorld(shared_from_this());

		int keyIdx = m_gameObjects[static_cast<int>(objectType)].size();
		m_objectMap.emplace(objName, std::make_pair(objectType, keyIdx));

		m_gameObjects[static_cast<int>(objectType)].emplace_back(obj);

		return obj;
	}
};

