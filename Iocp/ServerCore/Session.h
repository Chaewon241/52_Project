#pragma once
#include "NetAddress.h"
#include "IocpEvent.h"

/// <summary>
/// 클라이언트와 서버 각각 연결된 세션
/// </summary>

class Service;

class Session : public enable_shared_from_this<Session>
{
public:
	Session();
	~Session();

	bool Connect();

	void SetNetAddress(NetAddress address) { m_NetAddress = address; }
	NetAddress GetAddress() { return m_NetAddress; }

	void SetService(shared_ptr<Service> service) { m_Service = service; }
	shared_ptr<Service>	GetService() { return m_Service.lock(); }

	bool IsConnected() { return m_Connected; }
	void Dispatch(IocpEvent* iocpEvent, int32 numOfBytes);

	SOCKET GetSocket() { return m_Socket; }
	virtual HANDLE GetHandle() { return reinterpret_cast<HANDLE>(m_Socket); };
	shared_ptr<Session> GetSessionRef() { return static_pointer_cast<Session>(shared_from_this()); }

private:
	weak_ptr<Service>	m_Service;
	SOCKET m_Socket = INVALID_SOCKET;
	NetAddress m_NetAddress = {};
	atomic<bool> m_Connected = false;

// IOCP overlapped
private:
	ConnectEvent		m_ConnectEvent;
	DisconnectEvent		m_DisconnectEvent;
	RecvEvent			m_RecvEvent;
	SendEvent			m_SendEvent;
};