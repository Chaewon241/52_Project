// eventSelect.cpp WSAEventSelect 를 사용하여 비동기 소켓 프로그래밍을 해봅시다
// https://learn.microsoft.com/ko-KR/windows/win32/api/winsock2/nf-winsock2-wsaeventselect
#include <winsock2.h>

#include "LoopThread.h"

#include <ws2tcpip.h>
#include <iostream>
#include <vector>
// 순서 중요함다.

// link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

using namespace std;

constexpr int BUFSIZE = 1024;

//void OnError(errorType, WSAGetLastError())

class CNetClient : public CLoopThreadBase
{
public:
    CNetClient() {}
    virtual ~CNetClient() {}

    bool Connect();

private:
    virtual void DoUpdate() override;

    SOCKET    m_clientSocket = INVALID_SOCKET;

    WSAEVENT m_clientEvent = WSA_INVALID_EVENT;

    bool m_isConnected = false;

    char m_sendBuffer[BUFSIZE] = "똥닌자~";
};

struct Session
{
    SOCKET socket = INVALID_SOCKET; // 소켓 핸들

    char recvBuffer[BUFSIZE] = {};

    int recvBytes = 0;
    int sendBytes = 0; // 받은만큼 돌려 주는 인과응보 에코 서버
};

int main()
{
    WSAData wsaData;

    if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return 0;

    // 백로그에 하나
    // 
    SOCKET listenSocket = ::socket(AF_INET, SOCK_STREAM, 0);
    if (listenSocket == INVALID_SOCKET)
        return 0;

    SOCKADDR_IN serverAddr;
    ::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
    serverAddr.sin_port = ::htons(7777);

    if (::bind(listenSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
        return 0;

    // SOMAXCONN에서 1로 바꿔주기
    if (::listen(listenSocket, SOMAXCONN) == SOCKET_ERROR)
        return 0;

    cout << "Listen" << endl;

    // createevent
    vector<WSAEVENT> wsaEvents;
    // Session 구조체
    vector<Session> sessions;
    sessions.reserve(100);

    WSAEVENT listenEvent = ::WSACreateEvent();
    wsaEvents.push_back(listenEvent);               //[listenEvent][][][][]
    sessions.push_back(Session{ listenSocket });    //[listenSession][][][][]

    // 이벤트를 등록하고 해당 이벤트가 발생하면 인덱스를 알아올 수 있다.
    // WSAEventSelect 함수는 listenSocket를 넌블러킹 소켓으로 만든다.
    if (::WSAEventSelect(listenSocket, listenEvent, FD_ACCEPT) == SOCKET_ERROR) return 0;

    //더미 클라이언트 객체를 만들어서 스레드로 동작하게 해봅니다.
    CNetClient client;
    client.Start(true);

    if (false == client.Connect())
    {
        cout << "Connect Error " << WSAGetLastError() << endl;
    }

    while (true)
    {
        /*Sleep(1000);*/
        // 여기서 cmd창에 telnet ip 7777 하고 또 cmd 창에서 연결할라하면 안댐
        // 백로그.. accept를 안 해줘서..?

        // 인덱스를 가지고 어떤 이벤트인지 매치해줌. 근데 이렇게 하면 모든 이벤트가 켜질 수가 없어서 false
        // 영원히 기다리려고 WSA_INFINITE
        // 등록된 이벤트에 대해서 통보를 받기 위한 함수
        int index = ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], FALSE, WSA_INFINITE, FALSE);
        //int index = ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], FALSE, 1, FALSE);

        if (index == WSA_WAIT_FAILED)
            continue;
        // 이건 매뉴얼 입니당 원래 항상 WSA_WAIT_EVENT_0 더해줘서 하나 빼줘야함.
        index -= WSA_WAIT_EVENT_0;

        // WSAEventSelect로 소켓에 등록된 이벤트가 발생하면 어떤 이벤트인지 확인
        // 작업을 처리하는데 이벤트에 네트워크 종류를 확인해야함.
        // FD_ACCEPT

        // 네트워크 이벤트를 받아오것다!
        WSANETWORKEVENTS networkEvents;

        if (::WSAEnumNetworkEvents(sessions[index].socket, wsaEvents[index], &networkEvents) == SOCKET_ERROR)
            return 0;

        // 비트연산으로 해당 비트가 ACCEPT이면(이벤트가 발생했으면!)
        if (networkEvents.lNetworkEvents & FD_ACCEPT)   
        {
            // 뭔가 들어옴.
            // 근데 이벤트가 에러일 수도 있어서 체크 해줘야함.
            if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0) // FD_ACCEPT에 맞는 에러
                continue;

            // 에러가 아니면?
            // ACCEPT를 하겠다.

            // 클라이언트 소켓하나 주소 받아와서
            SOCKADDR_IN clientAddr;
            int addrLen = sizeof(clientAddr);
            // 클라이언트 주소 정보 제공
            SOCKET clientSocket = ::accept(listenSocket, (SOCKADDR*)&clientAddr, &addrLen);

            if (clientSocket != INVALID_SOCKET)
            {
                cout << "Client Connected" << endl;

                WSAEVENT clientEvent = ::WSACreateEvent();
                // 두번째로 이벤트 푸쉬백
                wsaEvents.push_back(clientEvent);
                sessions.push_back(Session{ clientSocket });

                // 클라랑 연결된거니까 READ해줌.
                if (::WSAEventSelect(clientSocket, clientEvent, FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR)
                    return 0;

                if (networkEvents.lNetworkEvents & FD_ACCEPT)
                {
                    if (networkEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
                    {
                        // FD_ACCEPT에 맞는 에러
                        OnError(errorType, WSAGetLastError()); // 앞에는 enum으로 Avvept인 connect 관련해서 정의하고
                        // 에러 관리에 대한 한 함수에서 일원화
                        OnAccept(WSAGetLastError());
                        continue;
                    }
                }
            }   
        }// FD_ACCEPT

        // 내가 소켓버퍼에 읽어올 데이터가 있다면(더이상 읽을게 없을 때만) || 
        if (networkEvents.lNetworkEvents & FD_READ || networkEvents.lNetworkEvents & FD_WRITE)
        {

            if ((networkEvents.lNetworkEvents & FD_READ) && networkEvents.iErrorCode[FD_READ_BIT] != 0)
                continue;


            if ((networkEvents.lNetworkEvents & FD_WRITE) && networkEvents.iErrorCode[FD_WRITE_BIT] != 0)
                continue;


            Session& s = sessions[index];

            // 다 읽었다.
            if (s.recvBytes == 0)
            {
                // receive 했을 때 길이
                int recvLen = ::recv(s.socket, s.recvBuffer, BUFSIZE, 0);
                if (recvLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK) {
                    continue;
                }

                if (recvLen > 0)
                    s.recvBytes = recvLen;

                cout << "Recv Data= " << recvLen << " : " << s.recvBuffer << endl;
            }

            // Send
            if (s.recvBytes > s.sendBytes) {
                int sendLen = ::send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);
                if (sendLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK) {
                    continue;
                }

                s.sendBytes += sendLen;
                if (s.recvBytes == s.sendBytes) {
                    s.recvBytes = 0;
                    s.sendBytes = 0;
                }

                cout << "Send Data= " << sendLen << endl;
            }
        }
    }

    WSACleanup();
    
    // 프로그램이 종료되면 어차피 끝나는 스레드지만 구색만 맞추기 스톱~
    client.Stop();

    return 0;
}

bool CNetClient::Connect()
{
    m_clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);

    if (m_clientSocket == INVALID_SOCKET) return false;

    m_clientEvent = ::WSACreateEvent();

    // 연결용

    // 해당 함수는 클라이언트 소켓 핸들에 FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE 4가지의 네트워크 신호가 켜지면
    // m_clientEvent 이벤트 객체를 통해서 알 수 있도록 연결 합니다.
    // m_clientSocket를 넌블러킹 모드로 바꿔도 됨.
    if (::WSAEventSelect(m_clientSocket, m_clientEvent, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR) return 0;

    SOCKADDR_IN serverAddr;
    ::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = ::htons(7777);
    inet_pton(AF_INET, "172.21.1.68", &(serverAddr.sin_addr)); // 다른 주소로 바꿔서 테스트 해보세요.

    if (::connect(m_clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) return false;

    cout << "Connecting" << endl;

    return true;
}

// 다형성, 가상 함수
void CNetClient::DoUpdate()
{
    // 소켓에 연결된 이벤트 객체의 통보를 대기 합니다..
    // 이벤트는 하나? 1000은 1초마다 기다림.
    int index = ::WSAWaitForMultipleEvents(1, &m_clientEvent, FALSE, 1000, FALSE);

    if (index == WSA_WAIT_FAILED)
        return;

    index -= WSA_WAIT_EVENT_0;

    WSANETWORKEVENTS networkEvents;

    if (::WSAEnumNetworkEvents(m_clientSocket, m_clientEvent, &networkEvents) == SOCKET_ERROR)
    {
        cout << "EnumNetworkEvents Error" << WSAGetLastError() << endl;
        return;
    }

    // 만약에 커넥트라면
    if (networkEvents.lNetworkEvents & FD_CONNECT)
    {
        if (networkEvents.iErrorCode[FD_CONNECT_BIT] != 0)
        {
            cout << "Connect Error " << networkEvents.iErrorCode[FD_CONNECT_BIT] << endl;
            return;
        }

        cout << "Connected" << endl;

        m_isConnected = true;
    }

    // receive 뭔가 읽어올게 있다면
    if(networkEvents.lNetworkEvents & FD_READ)
    {
        if (networkEvents.iErrorCode[FD_READ_BIT] != 0)
        {
            cout << "Read Error" << endl;
            return;
        }
        char buffer[BUFSIZE] = {};
        int recvBytes = ::recv(m_clientSocket, buffer, BUFSIZE, 0);
        if (recvBytes == SOCKET_ERROR)
        {
            cout << "Read Error" << endl;
            return;
        }

        // 읽을거 없을 때
        if (recvBytes == 0)
        {
            cout << "Disconnected" << endl;
            return;
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
}
