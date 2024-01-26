#pragma once

class UIObject;

class UIFunctor
{
public:
	virtual void Execute(const UIObject& onwer, float deltaTime) abstract;
};

