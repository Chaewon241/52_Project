#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"

/// <summary>
/// Listener�� ���������� IOCP�� ����� ���
/// </summary>

class Session : public IocpObject
{

	friend class Listener;
	friend class IocpCore;
	friend class Service;

public:
	Session();
	virtual ~Session();

public:
	void				Disconnect(const WCHAR* cause);

	shared_ptr<Service>	GetService() { return _service.lock(); }
	void				SetService(shared_ptr<Service> service) { _service = service; }

public:
	/* ���� ���� */
	void				SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress			GetAddress() { return _netAddress; }
	SOCKET				GetSocket() { return _socket; }
	// ���� ����Ǿ��ִ����� ���� ���������� ��Ŷ�� ��� recv ���� ������
	bool				IsConnected() { return _connected; }
	SessionRef			GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }

public:
	/* �������̽� ���� */
	virtual HANDLE		GetHandle() override;
	virtual void		Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	/* ���� ���� */
	// register -> �񵿱� �Լ��� �ɾ��ش�
	void				RegisterConnect();
	void				RegisterRecv();
	void				RegisterSend();
	// process -> ���������� register�Ǹ� ȣ��
	void				ProcessConnect();
	void				ProcessRecv(int32 numOfBytes);
	void				ProcessSend(int32 numOfBytes);
	// register�� process ���� ¦��� �ٴ�

	void				HandleError(int32 errorCode);

protected:
	/* ������ �ڵ忡�� �����ε� */
	virtual void		OnConnected() { }
	virtual int32		OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual void		OnSend(int32 len) { }
	virtual void		OnDisconnected() { }

public:
	// TEMP
	char _recvBuffer[1000];

private:
	// ���񽺸� ����ϰų� ���� �� �ֱ� ������ weak_ptr��
	weak_ptr<Service>	_service;
	SOCKET			_socket = INVALID_SOCKET;
	NetAddress		_netAddress = {};
	// ���������� �ƴ���
	Atomic<bool>	_connected = false;

private:
	USE_LOCK;

	/* ���� ���� */

	/* �۽� ���� */

private:
	/* IocpEvent ���� */
	RecvEvent			_recvEvent;
};

