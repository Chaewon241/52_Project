#pragma once

#include <unordered_map>

class InputSystem
{
public:
	InputSystem() {};
	~InputSystem() {};

	void Initialize();
	void Update();

	bool isKeyPressed(char key) const;

private:
	std::unordered_map<char, bool> keys;
};
