#include "pch.h"
#include "Service.h"
#include "Session.h"

Service::Service(ServiceType type, NetAddress address)
	: m_Type(type), m_NetAddress(address)
{
	m_IocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	if (m_IocpHandle == NULL)
	{
		printf("CreateIoCompletionPort() failed to create I/O completion port: %d\n",
			GetLastError());
	}
}

Service::~Service()
{
	::CloseHandle(m_IocpHandle);
}

bool Service::DisPatch(uint32 timeoutMs)
{
	DWORD numOfBytes = 0;
	ULONG_PTR key = 0;
	IocpEvent* iocpEvent = nullptr;
	
	if (::GetQueuedCompletionStatus(m_IocpHandle, OUT & numOfBytes, OUT & key, OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeoutMs))
	{
		shared_ptr<Session> iocpObject = iocpEvent->m_Owner;
		iocpObject->Dispatch(iocpEvent, numOfBytes);
	}
	else
	{
		int32 errCode = ::WSAGetLastError();
		switch (errCode)
		{
		case WAIT_TIMEOUT:
			return false;
		default:
			shared_ptr<Session> iocpObject = iocpEvent->m_Owner;
			iocpObject->Dispatch(iocpEvent, numOfBytes);
			break;
		}
	}

	return true;
}

shared_ptr<Session> Service::CreateSession()
{
	shared_ptr<Session> session = make_shared<Session>();
	session->SetService(shared_from_this());
	::CreateIoCompletionPort(session->GetHandle(), m_IocpHandle, 0, 0);

	return session;
}

/*-----------------
	ClientService
------------------*/

ClientService::ClientService(NetAddress targetAddress)
	: Service(ServiceType::Client, targetAddress)
{
}

bool ClientService::Start()
{
	shared_ptr<Session> session = CreateSession();

	return session->Connect();
}