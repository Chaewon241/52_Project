#include "ServerSocket.h"
#include "ClientSocket.h"
#include "Session.h"
#include "WinSock.h"

ServerSocket::ServerSocket(Server* sever)
{
	m_server = sever;
}

ServerSocket::~ServerSocket()
{
	delete m_server;
}

bool ServerSocket::OnAccept()
{
	SOCKADDR_IN clientAddr;
	int addrLen = sizeof(clientAddr);

	SOCKET hClientSocket = ::accept(this->GetSocket(), (SOCKADDR*)&clientAddr, &addrLen);

	if (hClientSocket == INVALID_SOCKET)
	{
		m_error = ::WSAGetLastError();
		return;
	}

	ClientSocket* pClient = new ClientSocket();
	Session* pSession = new Session();

	pClient->Attach(hClientSocket);

	char ipv4AddressAsString[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(clientAddr.sin_addr), ipv4AddressAsString, INET_ADDRSTRLEN);
	pClient->SetIp(ipv4AddressAsString);
	pClient->SetPortNum(::ntohs(clientAddr.sin_port));

	WSAEVENT clientEvent = ::WSACreateEvent();
	pClient->AddEvent(clientEvent);

	if (::WSAEventSelect(hClientSocket, clientEvent, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
	{
		m_error = ::WSAGetLastError();
		WSACloseEvent(clientEvent);
		closesocket(hClientSocket);

		delete pClient;
		delete pSession;

		return false;
	}

	m_server->AddClientSocket(pClient);
	m_server->AddSession(pSession, pClient);

	return true;
}