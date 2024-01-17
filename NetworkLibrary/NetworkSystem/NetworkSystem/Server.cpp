#include "Server.h"
#include "ClientSocket.h"
#include "ServerSocket.h"
#include "Session.h"
#include "WinSock.h"

void Server::Start()
{
	m_pServerSocket = new ServerSocket;

	m_networkSystem.Initialize(dynamic_cast<WinSock*>(m_pServerSocket));

	if (!m_pServerSocket->Create(SOCK_STREAM, FD_ACCEPT)
		|| !m_pServerSocket->Bind(7777)
		|| !m_pServerSocket->Listen())
		return;
}

void Server::Stop()
{
	delete m_pServerSocket;
	m_pServerSocket = nullptr;

	for (auto& pClient : m_clients)
	{
		delete pClient;
	}

	for (auto& pSession : m_sessions)
	{
		delete pSession.second;
	}
}

void Server::Update()
{
	if (m_pServerSocket == nullptr) return;

	for (auto& pClient : m_clients)
	{
		if (pClient == nullptr)
		{
			//std::cout << "nullptr " << std::endl;
		}
		else
		{
			//std::cout << "session id is " << pClient->GetSessionId() << std::endl;;
			m_pServerSocket->GetWSAEvents().push_back(pClient->GetWSAEvents()[0]);
		}
	}

	// 생각해 보기: 이벤트를 기다리는 시간을 1ms로 하면 호출 스레드는 대기 상태가 됩니다.
	// 메인 스레드(혹은 랜더 스레드)에서 딜레이를 주고 싶지 않을 경우, 어떻게 해야 할까요?
	int index = ::WSAWaitForMultipleEvents(m_pServerSocket->GetWSAEvents().size(), &m_pServerSocket->GetWSAEvents()[0], FALSE, 1, FALSE);

	if (index == WSA_WAIT_FAILED)
	{
		printf("WSAWaitForMultipleEvents Error %d", WSAGetLastError());
		return;
	}

	if (index == WSA_WAIT_TIMEOUT) return;

	index -= WSA_WAIT_EVENT_0;

	AsyncSocket* pSocket = nullptr;

	if (wsaEvents[index] == m_pListener->GetEvent()) pSocket = m_pListener;
	else pSocket = m_clients[index - 1];

	WSANETWORKEVENTS networkEvents;

	if (::WSAEnumNetworkEvents(pSocket->GetHandle(), wsaEvents[index], &networkEvents) == SOCKET_ERROR)
	{
		printf("WSAEnumNetworkEvents Error %d", WSAGetLastError());

		return;
	}

	if (networkEvents.lNetworkEvents & FD_ACCEPT)
	{
		if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
		{
			onNetError(networkEvents.iErrorCode[FD_ACCEPT_BIT], "Accept");
			return;
		}

		onAccept();
	}

	if (networkEvents.lNetworkEvents & FD_READ)
	{
		if (networkEvents.iErrorCode[FD_READ_BIT] != 0)
		{
			onNetError(networkEvents.iErrorCode[FD_READ_BIT], "Recv", pSocket);
			return;
		}

		onReceive(pSocket);
	}

	if (networkEvents.lNetworkEvents & FD_WRITE)
	{
		if (networkEvents.iErrorCode[FD_WRITE_BIT] != 0)
		{
			onNetError(networkEvents.iErrorCode[FD_WRITE_BIT], "Write", pSocket);
			return;
		}

		onSend(pSocket);
	}

	if (networkEvents.lNetworkEvents & FD_CLOSE)
	{
		if (networkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
		{
			onNetError(networkEvents.iErrorCode[FD_CLOSE_BIT], "Close", pSocket);
		}

		onClose(pSocket);
	}
	
}

void Server::OnReceive()
{
}

void Server::OnSend()
{
}

void Server::ServerLoop()
{
	Start();
	m_networkSystem.Initialize(m_pServerSocket);

	while (true)
	{
		Update();
	}
}
