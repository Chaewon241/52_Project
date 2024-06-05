#include "pch.h"
#include "Session.h"

#include "Service.h"

Session::Session()
{
	m_Socket = ::WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
}

Session::~Session()
{
	if (m_Socket != INVALID_SOCKET)
		::closesocket(m_Socket);
	m_Socket = INVALID_SOCKET;
}

bool Session::Connect()
{
	if (IsConnected())
		return false;

	if (GetService()->GetServiceType() != ServiceType::Client)
		return false;

	bool flag = true;
	if (SOCKET_ERROR == ::setsockopt(m_Socket, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<char*>(flag), sizeof(flag)))
		return false;

	SOCKADDR_IN myAddress;
	myAddress.sin_family = AF_INET;
	// INADDR_ANY : 아무거나 연결해줘
	myAddress.sin_addr.s_addr = ::htonl(INADDR_ANY);
	myAddress.sin_port = ::htons(0);

	return SOCKET_ERROR != ::bind(m_Socket, reinterpret_cast<const SOCKADDR*>(&myAddress), sizeof(myAddress));

	// overlapped 초기화
	m_ConnectEvent.Init();
	m_ConnectEvent.m_Owner = shared_from_this();

	// ConnectEx
	LPFN_CONNECTEX lpfnConnectEx = NULL;
	DWORD numOfBytes = 0;
	SOCKADDR_IN sockAddr = GetService()->GetNetAddress().GetSockAddr();
	BOOL success = lpfnConnectEx(
		m_Socket,
		reinterpret_cast<SOCKADDR*>(&sockAddr), 
		sizeof(sockAddr),
		nullptr,
		0,
		&numOfBytes,
		&m_ConnectEvent
	);

	if (!success)
		return false;

	int32 errorCode = ::WSAGetLastError();
	if (errorCode != WSA_IO_PENDING)
		return false;	
	
	return true;
}

void Session::Dispatch(IocpEvent* iocpEvent, int32 numOfBytes)
{
	switch (iocpEvent->m_EventType)
	{
	case EventType::Accept:
		//ProcessConnect();
		break;
	case EventType::Connect:
		//ProcessConnect();
		break;
	case EventType::Disconnect:
		//ProcessDisconnect();
		break;
	case EventType::Recv:
		//ProcessRecv(numOfBytes);
		break;
	case EventType::Send:
		//ProcessSend(numOfBytes);
		break;
	default:
		break;
	}
}