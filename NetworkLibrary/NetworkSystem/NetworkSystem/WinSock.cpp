#include "WinSock.h"

bool WinSock::Create(int nSocketType, long lEvent)
{
    m_hSocket = ::socket(AF_INET, nSocketType, 0);

    if (m_hSocket == INVALID_SOCKET)
        return false;

    WSAEVENT m_hEvent = ::WSACreateEvent();
    if (m_hEvent == WSA_INVALID_EVENT) return false;

    if (::WSAEventSelect(m_hSocket, m_hEvent, lEvent) == SOCKET_ERROR)
    {
        m_error = ::WSAGetLastError();
        return false;
    }
    m_WSAEvent.push_back(m_hEvent);
    return true;
}

bool WinSock::Bind(UINT nSocketPort)
{
    SOCKADDR_IN serverAddr;
    ::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
    serverAddr.sin_port = ::htons(nSocketPort);

    if (::bind(m_hSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        m_error = ::WSAGetLastError();
        return false;
    }
    return true;
}

bool WinSock::Listen(int nConnectionBacklog)
{
    if (::listen(m_hSocket, nConnectionBacklog) == SOCKET_ERROR)
    {
        m_error = ::WSAGetLastError();
        return false;
    }
    return true;
}

bool WinSock::Connect(UINT nSocketPort, const char* nIP)
{
    SOCKADDR_IN serverAddr;
    ::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    // todo: 포트번호
    serverAddr.sin_port = ::htons(nSocketPort);
    inet_pton(AF_INET, nIP, &(serverAddr.sin_addr));

    if (::connect(m_hSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) return false;

    return true;
}

bool WinSock::DisConnect()
{
    int iResult = shutdown(m_hSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
        return false;
    return true;
}

int WinSock::GetEventIndex()
{
    return ::WSAWaitForMultipleEvents(m_WSAEvent.size(), &m_WSAEvent[0], FALSE, 1, FALSE);
}
