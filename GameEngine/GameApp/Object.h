#pragma once
#include <vector>

class D2DRenderer;
class Object
{
public:
	Object();
	~Object();

	virtual void Update();
	virtual void Render(D2DRenderer* renderer);
};