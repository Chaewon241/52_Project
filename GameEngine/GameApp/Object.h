#pragma once
#include <vector>

class D2DRenderer;
class Object
{
public:
	Object();
	~Object();

	virtual void Update(float deltatime);
	virtual void Render(D2DRenderer* renderer);
protected:
	D2D1_VECTOR_2F m_Position;
};