#pragma once

/// <summary>
/// ���������� �ֻ����� �־ �󸶳� ������ �ϰ� �ִ���
/// Ȯ���� �� �ְ� ���ִ� Ŭ����
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
	// �����ϰ� �ִ� �ֵ��� �������
	int32 _refCount;
};

