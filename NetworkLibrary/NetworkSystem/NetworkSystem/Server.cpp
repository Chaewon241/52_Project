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
	m_networkSystem.CleanUpSock();
}

void Server::Update()
{
	std::vector<WSAEVENT> wsaEvents;

	wsaEvents.push_back(m_pServerSocket->GetEvent());

	for (auto& pClient : m_clients)
	{
		if (pClient == nullptr)
		{
			//std::cout << "nullptr " << std::endl;
		}
		else
		{
			//std::cout << "session id is " << pClient->GetSessionId() << std::endl;;

			wsaEvents.push_back(pClient->GetEvent());
		}
	}

	int index = ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], FALSE, 1, FALSE);

	if (index == WSA_WAIT_FAILED)
	{
		printf("WSAWaitForMultipleEvents Error %d", WSAGetLastError());
		return;
	}

	if (index == WSA_WAIT_TIMEOUT) return;

	index -= WSA_WAIT_EVENT_0;

	WinSock* pSocket = nullptr;

	if (wsaEvents[index] == m_pServerSocket->GetEvent()) pSocket = m_pServerSocket;
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

		OnAccept();
	}

	if (networkEvents.lNetworkEvents & FD_READ)
	{
		if (networkEvents.iErrorCode[FD_READ_BIT] != 0)
		{
			onNetError(networkEvents.iErrorCode[FD_READ_BIT], "Recv", pSocket);
			return;
		}

		OnReceive(pSocket);
	}

	if (networkEvents.lNetworkEvents & FD_WRITE)
	{
		if (networkEvents.iErrorCode[FD_WRITE_BIT] != 0)
		{
			onNetError(networkEvents.iErrorCode[FD_WRITE_BIT], "Write", pSocket);
			return;
		}

		OnSend(pSocket);
	}

	if (networkEvents.lNetworkEvents & FD_CLOSE)
	{
		if (networkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
		{
			onNetError(networkEvents.iErrorCode[FD_CLOSE_BIT], "Close", pSocket);
		}

		OnClose(pSocket);
	}
}

void Server::OnAccept()
{
	ClientSocket* pClient = new ClientSocket();
	Session* pSession = new Session();

	if (m_pServerSocket->OnAccept(pClient))
	{
		pSession->SetClient(pClient);

		m_clients.push_back(pClient);
		m_sessions[pSession->GetSessionId()] = pSession;

		++m_ClientCount;
	}
	else
	{
		delete pClient;
		delete pSession;

		printf("Accept Error %d\n", WSAGetLastError());
	}

	printf("연결된 클라이언트 수 : %d \n", m_ClientCount);
}

void Server::OnReceive(WinSock* pSocket)
{
	printf("onReceive  %s : %d\n", pSocket->GetIP().c_str(), pSocket->GetPort());

	ClientSocket* pClient = dynamic_cast<ClientSocket*>(pSocket);
	if (pClient == nullptr) return;

	Session* pSession = m_sessions[pClient->GetSessionId()];
	if (pSession == nullptr) return;

	pSession->ReadUpdate();

	printf("readBuffer: %s \n", pSession->GetReadBuffer());
}

void Server::OnSend(WinSock* pSocket)
{
	printf("onSend  %s : %d\n", pSocket->GetIP().c_str(), pSocket->GetPort());
}

void Server::OnClose(WinSock* pSocket)
{
	printf("onClose  %s : %d", pSocket->GetIP().c_str(), pSocket->GetPort());

	// todo : erase 고치기
	ClientSocket* pClient = dynamic_cast<ClientSocket*>(pSocket);
	if (pClient == nullptr) return;

	m_sessions.erase(pClient->GetSessionId());

	for (auto iter = m_clients.begin(); iter != m_clients.end(); ++iter)
	{
		if (*iter == pClient)
		{
			m_clients.erase(iter);
			break;
		}
	}

	delete pClient;

	m_ClientCount--;
	printf("연결된 클라이언트 수 : %d \n", m_ClientCount);
}

void Server::onNetError(int errorCode, const char* errorMsg, WinSock* pSocket)
{
	if (errorMsg)
	{
		printf("\n onNetError %s \t", errorMsg);
	}

	if (pSocket)
	{
		printf("\n onNetError  %s : %d", pSocket->GetIP().c_str(), pSocket->GetPort());
	}

	printf("NetErrorCode  %d  \n", errorCode);

	for (auto& session : m_sessions)
	{
		session.second->NetUpdate();
	}
}

void Server::ServerLoop()
{
	Start();
	m_networkSystem.Initialize(m_pServerSocket);

	while (true)
	{
		Update();
	}
	Stop();
}
