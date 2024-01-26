#include "pch.h"
#include "Server.h"

#include "Client.h"
#include "Session.h"

#include "../NetworkLibrary/ClientSocket.h"
#include "../NetworkLibrary/ServerSocket.h"
#include "../NetworkLibrary/WinSock.h"
#include "../Engine/WorldManager.h"

void Server::Start()
{
	WSAData wsaData;

	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return;

	m_pListenSocket = std::make_shared<ServerSocket>();

	if (!m_pListenSocket->Create(SOCK_STREAM, FD_ACCEPT)
		|| !m_pListenSocket->Bind(7777, "172.21.1.81")
		|| !m_pListenSocket->Listen())
		return;
}

void Server::Stop()
{
	::WSACleanup();
}

void Server::Update()
{
	std::vector<WSAEVENT> wsaEvents;

	wsaEvents.push_back(m_pListenSocket->GetEvent());

	for (auto& pClient : m_peerClients)
	{
		if (pClient == nullptr)
		{

		}
		else
		{
			std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
			wsaEvents.push_back(soc->GetEvent());
		}
	}

	if (IsAllReady())
	{
		for (auto& pClient : m_peerClients)
		{
			if (pClient == nullptr)
			{

			}
			else
			{
				std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
				std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];
				if (pSession == nullptr) return;

				char* ready = new char[sizeof(PacketS2C_READY) + 1];
				ready[0] = sizeof(PacketS2C_READY) / 10 + '0';
				ready[1] = sizeof(PacketS2C_READY) % 10 + '0';
				ready[2] = S2C_START / 10 + '0';
				ready[3] = S2C_START % 10 + '0';
				ready[4] = 1;
				ready[5] = '\0';

				pSession->PushSendQueue(ready, sizeof(PacketS2C_READY));
			}
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

	std::shared_ptr<WinSock> pSocket = nullptr;

	if (wsaEvents[index] == m_pListenSocket->GetEvent())
		pSocket = m_pListenSocket;
	else 
		pSocket = m_peerClients[index - 1]->GetSocket();

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

		OnClose(m_peerClients[index - 1]);
	}
}

void Server::NetUpdate()
{
	for (auto& session : m_sessions)
	{
		session.second->NetUpdate();
	}
}

void Server::OnAccept()
{
	std::shared_ptr<ClientNetworkManager> pClient = std::make_shared<ClientNetworkManager>();
	pClient->SetSocket(std::make_shared<ClientSocket>());
	std::shared_ptr<Session> pSession = std::make_shared<Session>();

	if (m_pListenSocket->OnAccept(pClient->GetSocket()))
	{
		pSession->SetClient(pClient);
		// 처음 들어온 클라이언트 Host 만들어주기.
		if (m_ClientCount == 0)
			pSession->SetHostSession();

		m_peerClients.push_back(pClient);
		m_sessions[pSession->GetSessionId()] = pSession;

		++m_ClientCount;
	}
	else
	{
	}
}

void Server::OnReceive(std::shared_ptr<WinSock> pSocket)
{
	printf("onReceive  %s : %d\n", pSocket->GetIP().c_str(), pSocket->GetPort());
	
	std::shared_ptr<ClientNetworkManager> pClient = std::make_shared<ClientNetworkManager>();
	std::shared_ptr<ClientSocket> clientSocket = std::dynamic_pointer_cast<ClientSocket>(pSocket);
	if (!clientSocket)
	{
		return;
	}
	pClient->SetSocket(clientSocket);

	if (pClient->GetSocket() == nullptr) return;

	std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];
	if (pSession == nullptr) return;

	pSession->ReadUpdate();

	if (pSession->IsRecvQueueEmpty())
		return;
	printf("readBuffer: %s \n", pSession->GetRecvQueueFrontBuffer());
}

void Server::OnSend(std::shared_ptr<WinSock> pSocket)
{
	printf("onSend  %s : %d\n", pSocket->GetIP().c_str(), pSocket->GetPort());
}

void Server::OnClose(std::shared_ptr<ClientNetworkManager> pClient)
{
	//printf("onClose  %s : %d", pSocket->GetIP().c_str(), pSocket->GetPort());

	//// todo 채원: erase 고치기
	//std::shared_ptr<ClientNetworkManager> pClient = std::make_shared<ClientNetworkManager>();
	//// 여기
	//std::shared_ptr<ClientSocket> clientSocket = std::dynamic_pointer_cast<ClientSocket>(pSocket);
	//if (!clientSocket)
	//{
	//	return;
	//}
	//pClient->SetSocket(clientSocket);

	if (pClient->GetSocket() == nullptr) return;

	m_sessions[pClient->GetSessionId()].get()->Finalize();

	m_sessions.erase(pClient->GetSessionId());

	for (auto iter = m_peerClients.begin(); iter != m_peerClients.end(); ++iter)
	{
		if (iter->get() == pClient.get())
		{
			m_peerClients.erase(iter);
			break;
		}
	}
	m_ClientCount--;
	printf("연결된 클라이언트 수 : %d \n", m_ClientCount);
}

void Server::onNetError(int errorCode, const char* errorMsg, std::shared_ptr<WinSock> pSocket)
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
}

// 모두가 Ready인지 체크
bool Server::IsAllReady()
{
	if (m_peerClients.size() != 2)
		return false;
	for (auto& pClient : m_peerClients)
	{
		if (pClient == nullptr)
		{

		}
		else
		{
			std::shared_ptr<ClientSocket> soc = pClient->GetSocket();
			std::shared_ptr<Session> pSession = m_sessions[pClient->GetSessionId()];
			if (pSession == nullptr) return false;
			if (!pSession->GetReadyState())
				return false;
		}
	}
	return true;
}
