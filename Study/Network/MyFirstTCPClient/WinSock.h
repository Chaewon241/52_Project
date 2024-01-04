#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>

#pragma comment (lib, "Ws2_32.lib")


//// 각 클라이언트와 서버 간의 연결 또는 상호작용
//struct Session
//{
//    SOCKET socket = INVALID_SOCKET; // 소켓 핸들
//
//    char recvBuffer[BUFSIZE] = {};
//
//    int recvBytes = 0;
//    int sendBytes = 0; // 받은만큼 돌려 주는 인과응보 에코 서버
//};

// 정말 가볍게 Winsock API 래핑 클래스를 만들어보자.
// 공통적으로 필요한 데이터와 함수만 정의해놓자.
// 얘가 지금 CAsyncSocket
class WinSock // 우리는 일단 비동기 소켓을 만들어야하기 때문에 WSAEventSelect를 사용한다.
{
public:
    // 재사용성
        // 공통 API
        // 비동기 소켓이므로 생성할 때, 필요한 네트워크 이벤트를 설정 하도록 한다.
        // 소켓 생성 Create : long lEvent
    bool Create(int nSocketType);
    // 주소 바인드
    bool Bind(UINT nSocketPort);
    // listen
    bool Listen(int nConnectionBacklog);
    // 연결 Connect
    bool Connect(UINT nSocketPort, const char* nIP);
    bool DisConnect();

    bool CreateEventHandle();

    WSAEVENT GetEventHandle()
    {
        return m_WSAEvent;
    }

    SOCKET GetSocket()
    {
        return m_hSocket;
    }

    virtual void OnAccept(int nErrorCode) {};      // 순수 가상으로 만들었는데
    virtual void OnClose(int nErrorCode) {};
    virtual void OnConnect(int nErrorCode) {};
    virtual void OnReceive(int nErrorCode) {};
    virtual void OnSend(int nErrorCode) {};

protected:
    SOCKET m_hSocket;

    WSAEVENT m_WSAEvent;
};


class MyWinSocket : public WinSock
{
public:
    virtual void OnReceive(int nErrorCode) {}

    // 받은 패킷을 분석해서 처리가 가능한 메시지로 분리하고
    // 큐에 넣는다. 큐에 처리가 가능한 메시지가 완성되었다면.
    // 
    // OnMsgProc(&msg)
    //

    virtual void OnAccept(int nErrorCode) {};
    virtual void OnClose(int nErrorCode) {};
    virtual void OnConnect(int nErrorCode);
    virtual void OnSend(int nErrorCode);


    void OnMsgProc(void* pMsg)
    {
        // msg의 헤더의 패킷 아이디를 확인하고 각각에 맞게 처리하는 함수를 호출한다.
    }
};