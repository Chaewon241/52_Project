#pragma once
#include <vector>
#include <typeinfo>
#include "json.hpp"

class D2DRenderer;
class Object
{
public:
	Object();
	~Object();

	virtual void Update();
	virtual void Render(D2DRenderer* renderer);

	std::string GetClassName()
	{
		const std::type_info& self = typeid(*this);
		return self.name();
	}

	virtual void SerializeOut(nlohmann::ordered_json & object) = 0;
};