#include "WinSock.h"


// �񵿱� ���� �����ϰ� �ʱ�ȭ�ϴ� �Լ�
bool WinSock::Create(int nSocketType)
{
    m_hSocket = ::socket(AF_INET, nSocketType, 0);
    if (m_hSocket == INVALID_SOCKET)
        return false;

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

        return false;
    }
        

    return true;
}

bool WinSock::Listen(int nConnectionBacklog)
{
    if (::listen(m_hSocket, nConnectionBacklog) == SOCKET_ERROR)
        return false;
    return true;
}

bool WinSock::CreateEventHandle()
{
    // �̺�Ʈ �������ְ�
    WSAEVENT listenEvent = ::WSACreateEvent();
    // �̺�Ʈ�� ������ ���� pushback ���ش�.
    wsaEvents.push_back(listenEvent);
    sessions.push_back(Session{ m_hSocket });

    // ���Ͽ� �̺�Ʈ ����
    if (::WSAEventSelect(m_hSocket, listenEvent, FD_ACCEPT) == SOCKET_ERROR) return false;
    return true;
}

int WinSock::WaitForMultipleEvents()
{
    index = ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], FALSE, WSA_INFINITE, FALSE);

    if (index == WSA_WAIT_FAILED)
        return WSA_WAIT_FAILED;

    index -= WSA_WAIT_EVENT_0;
    return index;
}

bool WinSock::EnumNetworkEvents()
{
    if (::WSAEnumNetworkEvents(sessions[index].socket, wsaEvents[index], &networkEvents) == SOCKET_ERROR)
        return false;
    return true;
}

void MyWinSocket::OnAccept(int nErrorCode)
{
    // Ŭ���̾�Ʈ �����ϳ� �ּ� �޾ƿͼ�
    SOCKADDR_IN clientAddr;
    int addrLen = sizeof(clientAddr);
    // Ŭ���̾�Ʈ �ּ� ���� ����
    SOCKET clientSocket = ::accept(m_hSocket, (SOCKADDR*)&clientAddr, &addrLen);


    // ���� onconnect�� ���� ���ֱ�
    if (clientSocket != INVALID_SOCKET)
    {
        cout << "Client Connected" << endl;

        WSAEVENT clientEvent = ::WSACreateEvent();

        wsaEvents.push_back(clientEvent);
        sessions.push_back(Session{ clientSocket });

        if (::WSAEventSelect(clientSocket, clientEvent, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
            return;
    }
}

