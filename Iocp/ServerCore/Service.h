#pragma once
#include "NetAddress.h"

enum class ServiceType : unsigned __int8
{
	Server,
	Client
};

class Session;

class Service : public enable_shared_from_this<Service>
{
public:
	Service(ServiceType type, NetAddress address);
	virtual ~Service();

	virtual bool Start() abstract;
	virtual void CloseService();

	bool DisPatch(uint32 timeoutMs = INFINITE);

	shared_ptr<Session> CreateSession();

public:
	ServiceType GetServiceType() { return m_Type; }

	NetAddress GetNetAddress() { return m_NetAddress; }


private:
	ServiceType m_Type;
	NetAddress m_NetAddress;

	set<shared_ptr<Session>> m_Sessions;
	HANDLE m_IocpHandle;
};

/*-----------------
	ClientService
------------------*/

class ClientService : public Service
{
public:
	ClientService(NetAddress targetAddress);
	virtual ~ClientService() {}

	virtual bool	Start() override;
};

/*-----------------
	ServerService
------------------*/

//class ServerService : public Service
//{
//public:
//	ServerService(NetAddress targetAddress);
//	virtual ~ServerService() {}
//
//	virtual bool	Start() override;
//	virtual void	CloseService() override;
//
//private:
//	//ListenerRef		_listener = nullptr;
//};