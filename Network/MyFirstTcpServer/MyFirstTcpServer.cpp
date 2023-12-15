﻿// 맨 처음에 해야하는거

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>

#pragma comment (lib, "Ws2_32.lib")

using namespace std;

constexpr int BUFSIZE = 1024;

namespace net_global
{
    bool WinsockInit()
    {

    }

    void WinsockCleanUp()
    {

    }
}

// 각 클라이언트와 서버 간의 연결 또는 상호작용
struct Session
{
    SOCKET socket = INVALID_SOCKET; // 소켓 핸들

    char recvBuffer[BUFSIZE] = {};

    int recvBytes = 0;
    int sendBytes = 0; // 받은만큼 돌려 주는 인과응보 에코 서버
};

// 정말 가볍게 Winsock API 래핑 클래스를 만들어보자.
// 공통적으로 필요한 데이터와 함수만 정의해놓자.
// 얘가 지금 CAsyncSocket
class WinSocket // 우리는 일단 비동기 소켓을 만들어야하기 때문에 WSAEventSelect를 사용한다.
{
public:
// 재사용성
    // 공통 API
    // 비동기 소켓이므로 생성할 때, 필요한 네트워크 이벤트를 설정 하도록 한다.
    // 소켓 생성 Create : long lEvent
    bool Create(int nSocketType);
    // 주소 바인드
    bool Bind(UINT nSocketPort, LPCTSTR lpszSocketAddress = NULL);
    // listen
    bool Listen(int nConnectionBacklog);
    // 연결 Connect
    
    // 닫기 closesocket : 소켓 닫을 때도 옵션이 있다. 디폴트 옵션으로 닫는다.

    // Accept
    // API만 래핑해서 만들어두고 상속 받은 클래스의 OnAccept에서 해당 함수를 호출

    // Attach와 Detach가 필요한 이유
    // 소켓 풀을 만들 때(소켓 핸들 생성전) 미리 만들어서 
    // Accept의 결과가 소켓 핸들을 받아서 소켓 객체 풀에서 하나 꺼내서 Attach(바인드)하고
    // 소켓이 닫히면 해당 소켓 핸들은 더이상 사용하지 않으나 객체는 다시 사용해야하므로
    // Detach하고 풀로 돌려준다.
    // 
    
    bool CreateEventHandle();
    bool WaitForMultipleEvents();
    bool EnumNetworkEvents();
    bool EnumNetworkEvents();
        
    UINT GetEventState(){ return networkEvents.lNetworkEvents; }
    int GetEventErrorCode(){ return networkEvents.iErrorCode[FD_ACCEPT_BIT]; }

    // 소켓 상태 : WinSocket으로 객체를 여러개 미리 만들어둘 수 있다. 소켓 객체 풀
    // 소켓 생성전. (socket(..))

// 다형성
    virtual void OnAccept(int nErrorCode) = 0;      // 순수 가상으로 만들었는데
    virtual void OnClose(int nErrorCode) = 0;
    virtual void OnConnect(int nErrorCode) = 0;
    virtual void OnReceive(int nErrorCode) = 0;
    virtual void OnSend(int nErrorCode) = 0;

protected:
    SOCKET m_hSocket;

    vector<WSAEVENT> wsaEvents;
    vector<Session> sessions;

    // 이벤트의 인덱스
    int index;

    // 소켓의 이벤트 정보, 이벤트에 대한 오류 코드 배열이 있음.
    WSANETWORKEVENTS networkEvents;
};

class MyWinSocket : public WinSocket
{
public:
    virtual void OnReceive(int nErrorCode)
    {
        // 받은 패킷을 분석해서 처리가 가능한 메시지로 분리하고
        // 큐에 넣는다. 큐에 처리가 가능한 메시지가 완성되었다면.
        // 
        // OnMsgProc(&msg)
        //
    }

    virtual void OnAccept(int nErrorCode);
    virtual void OnClose(int nErrorCode) {}
    virtual void OnConnect(int nErrorCode) {}
    virtual void OnSend(int nErrorCode);


    void OnMsgProc(void* pMsg)
    {
        // msg의 헤더의 패킷 아이디를 확인하고 각각에 맞게 처리하는 함수를 호출한다.
    }
};

// 자주 쓰는거 래핑

int main()
{
    // 윈속 초기화는 프로그램 실행시에 한번만 해야한다.
    WSAData wsaData;

    if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return 0;

    MyWinSocket ws;

    UINT nSocketPort = 7777;

    ws.Create(SOCK_STREAM);
    ws.Bind(nSocketPort);
    ws.Listen(1);
    ws.CreateEventHandle();

    while (true)
    {
        if (!(ws.WaitForMultipleEvents()))
            continue;

        ws.EnumNetworkEvents();


        UINT eventState = ws.GetEventState();

        if (eventState == FD_ACCEPT)
        {
            int errorCode = ws.GetEventErrorCode();

            ws.OnAccept(errorCode);
        }

        // 내가 소켓버퍼에 읽어올 데이터가 있다면(더이상 읽을게 없을 때만) || 
        else if (eventState == FD_READ || eventState == FD_WRITE)
        {
            int errorCode = ws.GetEventErrorCode();
            
            ws.OnSend(errorCode);
        }
    }

    // 해제도 한번만.
    WSACleanup();
    return 0;
}

// 비동기 소켓 관리하고 초기화하는 함수
bool WinSocket::Create(int nSocketType)
{
    m_hSocket = ::socket(AF_INET, nSocketType, 0);
    if (m_hSocket == INVALID_SOCKET)
        return false;

    return true;
}

bool WinSocket::Bind(UINT nSocketPort, LPCTSTR lpszSocketAddress = NULL)
{
    SOCKADDR_IN serverAddr;
    ::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = ::htonl(INADDR_ANY);
    serverAddr.sin_port = ::htons(nSocketPort);

    if (::bind(m_hSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
        return false;

    return true;
}

bool WinSocket::Listen(int nConnectionBacklog)
{
    if (::listen(m_hSocket, nConnectionBacklog) == SOCKET_ERROR)
        return false;
    return true;
}

bool WinSocket::CreateEventHandle()
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

bool WinSocket::WaitForMultipleEvents()
{
    index = ::WSAWaitForMultipleEvents(wsaEvents.size(), &wsaEvents[0], FALSE, WSA_INFINITE, FALSE);

    if (index == WSA_WAIT_FAILED)
        return false;

    index -= WSA_WAIT_EVENT_0;
    return true;
}

bool WinSocket::EnumNetworkEvents()
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

void MyWinSocket::OnSend(int nErrorCode)
{
    // continue를 어떻게 해결할까
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
