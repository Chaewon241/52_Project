#pragma once

#include <variant>
#include <typeinfo>

class EventListener;
enum class eEventType;
enum class eWorldType;

using eventVariant = std::variant<eWorldType>;

class Event
{
public:
	template <typename T>
	T GetData()
	{
		return std::get<T>(variants[variantMap.find(typeid(T).name())->second]);
	}

	template <typename... Args>
	Event(Args... args)
	{
		(InsertEventData(args), ...);
	}

private:
	template <typename T>
	void InsertEventData(T data)
	{
		variantMap.emplace(typeid(data).name(), variants.size());
		variants.emplace_back(data);
	}


public:
	eEventType eventID;
	EventListener* eventListener = nullptr;

private:
	std::vector<eventVariant> variants;
	std::unordered_map<std::string, int> variantMap;
};

