#pragma once


class Component : public Object
{
public:
	Component();
	virtual ~Component();
	
	virtual void Update();
	virtual void Render(D2DRenderer* renderer);
};