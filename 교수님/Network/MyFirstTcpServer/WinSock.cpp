#include "WinSock.h"


// 비동기 소켓 관리하고 초기화하는 함수
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
    // 이벤트 생성해주고
    WSAEVENT listenEvent = ::WSACreateEvent();
    // 이벤트랑 소켓을 각각 pushback 해준다.
    wsaEvents.push_back(listenEvent);
    sessions.push_back(Session{ m_hSocket });

    // 소켓에 이벤트 설정
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
    // 클라이언트 소켓하나 주소 받아와서
    SOCKADDR_IN clientAddr;
    int addrLen = sizeof(clientAddr);
    // 클라이언트 주소 정보 제공
    SOCKET clientSocket = ::accept(m_hSocket, (SOCKADDR*)&clientAddr, &addrLen);


    // 여기 onconnect로 따로 빼주기
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

