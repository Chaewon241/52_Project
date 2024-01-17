#include "Client.h"
#include "ClientSocket.h"

void Client::Start()
{
	m_clientSocket = new ClientSocket;
}

void Client::Update()
{
    int eventState = m_clientSocket->GetEventIndex();

    if (eventState == WSA_WAIT_FAILED)
        return;

    eventState -= WSA_WAIT_EVENT_0;

    if (eventState == WSA_WAIT_TIMEOUT)
        return;

    WSANETWORKEVENTS networkEvents;
    if (::WSAEnumNetworkEvents(m_clientSocket->GetSocket(), m_clientSocket->GetWSAEvents()[eventState], &networkEvents) == SOCKET_ERROR)
        return;

    if (networkEvents.lNetworkEvents & FD_CONNECT)
    {
        if (networkEvents.iErrorCode[FD_CONNECT_BIT] != 0)
        {
            return;
        }
    }

    if (networkEvents.lNetworkEvents & FD_READ)
    {
        if (networkEvents.iErrorCode[FD_READ_BIT] != 0)
        {
            return;
        }
        OnReceive();
    }

    if (networkEvents.lNetworkEvents & FD_WRITE)
    {
        if (networkEvents.iErrorCode[FD_WRITE_BIT] != 0)
        {
            //onNetError(networkEvents.iErrorCode[FD_WRITE_BIT], "Write", pSocket);
            return;
        }

        OnSend();
    }

    if (networkEvents.lNetworkEvents & FD_CLOSE)
    {
        if (networkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
        {
            return;
        }
    }
}

void Client::OnReceive()
{
    m_networkSystem.OnReceive();
}

void Client::OnSend()
{
}

void Client::Stop()
{
}

void Client::ClientLoop()
{
	Start();
	m_networkSystem.Initialize(m_clientSocket);

	while(true)
	{
        Update();
	}
}
