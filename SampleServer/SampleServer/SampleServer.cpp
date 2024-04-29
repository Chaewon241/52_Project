#include "pch.h"
#include "CorePch.h"
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>

#include "ThreadManager.h"
#include "RefCounting.h"

class Wraight : public RefCountable
{
public:
	int _hp = 150;
	int _posX = 150;
	int _posY = 150;
};

class Missile : public RefCountable
{
public:
	void SetTarget(Wraight* target)
	{
		_target = target;
		target->AddRef();
	}

	void Update()
	{
		int posX = _target->_posX;
		int posY = _target->_posY;
	}

	Wraight* _target = nullptr;
};

int main()
{
	Wraight* wraight = new Wraight();
	Missile* missile = new Missile();
	missile->SetTarget(wraight);
	
	// 레이스가 피격 당함.
	wraight->_hp = 0;
	delete wraight;

	while (true)
	{
		if (missile)
		{
			missile->Update();
		}
	}
}