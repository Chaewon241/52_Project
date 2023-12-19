#include "WinSockBase.h"

bool WinsockInit()
{
    return false;
}

void WinsockCleanUp()
{
}

WinSockBase::WinSockBase(void)
{
}

WinSockBase::~WinSockBase(void)
{
}

// 리스닝 소켓, 클라이언트 소켓, 
bool WinSockClient::Connect()
{
    m_clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);

    if (m_clientSocket == INVALID_SOCKET) return false;

    m_clientEvent = ::WSACreateEvent();

    if (::WSAEventSelect(m_clientSocket, m_clientEvent, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR) return 0;

    SOCKADDR_IN serverAddr;
    ::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = ::htons(7777);
    inet_pton(AF_INET, "172.21.1.81", &(serverAddr.sin_addr));  // 여기 주소 바꾸기

    if (::connect(m_clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) return false;

    return true;
}

bool WinSockClient::DoUpdate()
{
    // 이벤트 감지
    // 이벤트객체가 하나라도 실행 가능한 상태가 되었는지 확인.
    int index = ::WSAWaitForMultipleEvents(1, &m_clientEvent, FALSE, 1000, FALSE);
    // 실행 가능한 상태인 이벤트 중 가장 먼저 발견된 이벤트의 인덱스를 반환
    if (index == WSA_WAIT_FAILED)
        return false;

    index -= WSA_WAIT_EVENT_0;

    // 소켓의 내부 정보
    WSANETWORKEVENTS networkEvents;

    // WSAEnumNetworkEvents 함수는 소켓에 발생한 네트워크 이벤트를 확인하는 데 사용
    // 주로 WSAEvent와 함께 사용되어 소켓에서 발생한 이벤트가 있는지 확인하고 
    // 이벤트의 상태를 얻는 데 활용
    if (::WSAEnumNetworkEvents(m_clientSocket, m_clientEvent, &networkEvents) == SOCKET_ERROR)
    {
        cout << "EnumNetworkEvents Error" << WSAGetLastError() << endl;
        return false;
    }

    // 여기까지 순서: 이벤트 객체 상태 확인(WSAWaitForMultipleEvents)
    // -> 소켓에서 발생한 네트워크 이벤트에 대한 정보 얻기(WSAEnumNetworkEvents)

    // 만약에 커넥트라면
    if (networkEvents.lNetworkEvents & FD_CONNECT)
    {
        if (networkEvents.iErrorCode[FD_CONNECT_BIT] != 0)
        {
            cout << "Connect Error " << networkEvents.iErrorCode[FD_CONNECT_BIT] << endl;
            return false;
        }

        cout << "Connected" << endl;

        m_isConnected = true;
    }

    // receive 뭔가 읽어올게 있다면
    if (networkEvents.lNetworkEvents & FD_READ)
    {
        if (networkEvents.iErrorCode[FD_READ_BIT] != 0)
        {
            cout << "Read Error" << endl;
            return false;
        }
        char buffer[BUFSIZE] = {};
        int recvBytes = ::recv(m_clientSocket, buffer, BUFSIZE, 0);
        if (recvBytes == SOCKET_ERROR)
        {
            cout << "Read Error" << endl;
            return false;
        }

        // 읽을거 없을 때
        if (recvBytes == 0)
        {
            cout << "Disconnected" << endl;
            return false;
        }
        cout << "Recv: " << buffer << endl;
    }

    if (m_isConnected)
    {
        Sleep(1000);

        int sendLen = ::send(m_clientSocket, m_sendBuffer, strlen(m_sendBuffer) + 1, 0);
        if (sendLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK) {
            cout << "send Error " << ::WSAGetLastError() << endl;
        }
        else
        {
            cout << "send " << sendLen << " " << endl;
        }
    }
    return true;
}

void WinSockClient::CloseSocket()
{
    closesocket(m_clientSocket);
}

