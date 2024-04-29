#pragma once

/// <summary>
/// 컨텐츠들의 최상위에 있어서 얼마나 참조를 하고 있는지
/// 확인할 수 있게 해주는 클래스
/// </summary>

class RefCountable
{
public:
	RefCountable() : _refCount(1) {}
	virtual ~RefCountable() {}

	int32 GetRefCount() { return _refCount; }
	int32 AddRef() { ++_refCount; }
	int32 ReleaseRef()
	{
		int32 refCount = --_refCount;
		if (_refCount == 0)
		{
			delete this;
		}
		return refCount;
	}

protected:
	// 참조하고 있는 애들이 몇명인지
	int32 _refCount;
};

