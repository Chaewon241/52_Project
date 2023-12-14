// 맨 처음에 해야하는거

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include <vector>

#pragma comment (lib, "Ws2_32.lib")

using namespace std;

namespace net_global
{
    bool WinsockInit()
    {

    }

    void WinsockCleanUp()
    {

    }
}

// 정말 가볍게 Winsock API 래핑 클래스를 만들어보자.
// 공통적으로 필요한 데이터와 함수만 정의해놓자.
// 얘가 지금 CAsyncSocket
class WinSocket // 우리는 일단 비동기 소켓을 만들어야하기 때문에 WSAEventSelect를 사용한다.
{
public:
    SOCKET m_hSocket;

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

    

    // 소켓 상태 : WinSocket으로 객체를 여러개 미리 만들어둘 수 있다. 소켓 객체 풀
    // 소켓 생성전. (socket(..))

// 다형성
    virtual void OnAccept(int nErrorCode) = 0;      // 순수 가상으로 만들었는데
    virtual void OnClose(int nErrorCode) = 0;
    virtual void OnConnect(int nErrorCode) = 0;
    virtual void OnReceive(int nErrorCode) = 0;
    virtual void OnSend(int nErrorCode) = 0;
};

class MyWinSocket : public WinSocket
{
    virtual void OnReceive(int nErrorCode)
    {
        // 받은 패킷을 분석해서 처리가 가능한 메시지로 분리하고
        // 큐에 넣는다. 큐에 처리가 가능한 메시지가 완성되었다면.
        // 
        // OnMsgProc(&msg)
        //
    }

    virtual void OnAccept(int nErrorCode) {}
    virtual void OnClose(int nErrorCode) {}
    virtual void OnConnect(int nErrorCode) {}
    virtual void OnSend(int nErrorCode) {}


    void OnMsgProc(void* pMsg)
    {
        // msg의 헤서의 패킷 아이디를 확인하고 각각에 맞게 처리하는 함수를 호출한다.
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