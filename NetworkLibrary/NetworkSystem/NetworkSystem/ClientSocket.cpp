#include "ClientSocket.h"

void ClientSocket::Attach(SOCKET socket)
{
	m_hSocket = socket;
}

void ClientSocket::SetIp(std::string ip)
{
	m_ip = ip;
}

void ClientSocket::SetPortNum(int port)
{
	m_port = port;
}
