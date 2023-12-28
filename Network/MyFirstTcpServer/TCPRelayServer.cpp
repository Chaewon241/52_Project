#include "TCPRelayServer.h"
#include "Client.h"
#include "Session.h"
#include <cstdio>
#include <iostream>

// link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

namespace netfish
{
	class Listener : public AsyncSocket
	{
	public:
		Listener() = default;
		~Listener() = default;

		virtual bool OnAccept(AsyncSocket* pSocket) override
		{
			return (Accept(*pSocket));
		}
		virtual bool OnConnect() override { return false; }
		virtual bool OnClose() override { return false; }
		virtual bool OnSend() override { return false; }
		virtual bool OnReceive() override { return false; }
	};

	void TCPRelayServer::Start()
	{
		AsyncSocket::InitSock();

		m_pListener = new Listener();

		m_pListener->Create(SOCK_STREAM, FD_ACCEPT);

		m_pListener->Bind(7777, "127.0.0.1");
		
		m_pListener->Listen();
	}

	void TCPRelayServer::Stop()
	{
		delete m_pListener;
		m_pListener = nullptr;

		for (auto& pClient : m_clients)
		{
			delete pClient;
		}

		for (auto& pSession : m_sessions)
		{
			delete pSession.second;
		}
		AsyncSocket::CleanupSock();
	}

	void TCPRelayServer::NetUpdate()
	{
		if (m_pListener == nullptr) return;

		std::vector<WSAEVENT> wsaEvents;

		wsaEvents.push_back(m_pListener->GetEvent());

		for (auto& pClient : m_clients)
		{
			if (pClient == nullptr)
			{
				std::cout << "nullptr " << std::endl;
			}
			else
			{
				//std::cout << "session id is " << pClient->GetSessionId() << std::endl;;

				wsaEvents.push_back(pClient->GetEvent());
			}
		}

		// 생각해 보기: 이벤트를 기다리는 시간을 1ms로 하면 호출 스레드는 대기 상태가 됩니다.
		// 메인 스레드(혹은 랜더 스레드)에서 딜레이를 주고 싶지 않을 경우, 어떻게 해야 할까요?
		int index = ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], FALSE, 1, FALSE);

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

		for (auto& session : m_sessions)
		{
			session.second->NetUpdate();
		}


	}

	void TCPRelayServer::onAccept()
	{
		printf("onAccept\n");

		Client* pClient = new Client();
		Session* pSession = new Session();

		if (m_pListener->OnAccept(pClient))
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

	void TCPRelayServer::onReceive(AsyncSocket* pSocket)
	{
		printf("onReceive  %s : %d\n", pSocket->GetIP().c_str(), pSocket->GetPort());

		Client* pClient = dynamic_cast<Client*>(pSocket);
		if (pClient == nullptr) return;

		Session* pSession = m_sessions[pClient->GetSessionId()];

		if (pSession == nullptr) return;

		pSession->ReadUpdate();

		printf("readBuffer: %s \n", pSession->GetReadBuffer());
	}

	void TCPRelayServer::onSend(AsyncSocket* pSocket)
	{
		printf("onSend  %s : %d\n", pSocket->GetIP().c_str(), pSocket->GetPort());
	}

	void TCPRelayServer::onClose(AsyncSocket* pSocket)
	{
		printf("onClose  %s : %d", pSocket->GetIP().c_str(), pSocket->GetPort());

		// 클라이언트 소켓이 닫히면 클라이언트를 삭제합니다. 연결된 세션도 삭제 합니다.
		Client* pClient = dynamic_cast<Client*>(pSocket);
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
		//숙제: 이코드의 문제점은 무엇일까요?
		// 효율성

		delete pClient;


		m_ClientCount--;
		printf("연결된 클라이언트 수 : %d \n", m_ClientCount);
	}


	void TCPRelayServer::onNetError(int errorCode, const char* errorMsg, AsyncSocket* pSocket)
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
}