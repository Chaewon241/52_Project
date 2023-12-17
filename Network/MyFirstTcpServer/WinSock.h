#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>

#pragma comment (lib, "Ws2_32.lib")

using namespace std;

constexpr int BUFSIZE = 1024;

namespace net_global
{
    //bool WinsockInit()
    //{
    //    /*if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    //        return false;*/

    //    return true;
    //}

    //void WinsockCleanUp()
    //{

    //}
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
    bool Connect(UINT nSocketPort);
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
    int WaitForMultipleEvents();
    bool EnumNetworkEvents();

    UINT GetEventState() { return networkEvents.lNetworkEvents; }
    int* GetErrorCode() { return networkEvents.iErrorCode; }




    // 소켓 상태 : WinSocket으로 객체를 여러개 미리 만들어둘 수 있다. 소켓 객체 풀
    // 소켓 생성전. (socket(..))

// 다형성
    virtual void OnAccept(int nErrorCode) = 0;      // 순수 가상으로 만들었는데
    //virtual void OnClose(int nErrorCode) = 0;
    virtual void OnConnect(int nErrorCode) = 0;
    virtual void OnReceive(int nErrorCode) = 0;
    virtual bool OnSend(int nErrorCode) = 0;

protected:
    SOCKET m_hSocket;

    vector<WSAEVENT> wsaEvents;
    vector<Session> sessions;

    // 이벤트의 인덱스
    int index;

    // 소켓의 이벤트 정보, 이벤트에 대한 오류 코드 배열이 있음.
    WSANETWORKEVENTS networkEvents;
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

    virtual void OnAccept(int nErrorCode);
    //virtual void OnClose(int nErrorCode);
    virtual void OnConnect(int nErrorCode) {}
    virtual bool OnSend(int nErrorCode);


    void OnMsgProc(void* pMsg)
    {
        // msg의 헤더의 패킷 아이디를 확인하고 각각에 맞게 처리하는 함수를 호출한다.
    }
};