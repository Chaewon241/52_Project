#pragma once
#include "Object.h"
#include "D2DRenderer.h"

#include <vector>

class RectObject : public Object
{
private:
	RectObject* m_parent;
	std::vector<RectObject> m_Children;
public:
	RectObject();
	virtual ~RectObject();
public:
	virtual void Update(float deltatime) override;
	virtual void Render(D2DRenderer* renderer) override;

	void AddChild(RectObject* child);
};
