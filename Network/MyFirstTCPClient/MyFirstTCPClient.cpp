#include <iostream>
#include <vector>
#include <winsock2.h>
#include <ws2tcpip.h>

#include "WinSockBase.h"

#pragma comment (lib, "Ws2_32.lib")

using namespace std;


// CAsyncSocket을 참고해서 클래스로 만들어봅니다.
// 해당 클래스를 사용해서 서버에 연결하는 코드 작성
// eventSelect 예제를 참고해서 만들어봅니다.
// eventSelect 서버에 연결을 확인해봅니다.
//
class WinSockClient : public WinSockBase
{
public:
    WinSockClient() {}
    virtual ~WinSockClient() {}

    bool Connect();

    void CloseSocket();

private:
    virtual void DoUpdate() override;

    SOCKET    m_clientSocket = INVALID_SOCKET;

    WSAEVENT m_clientEvent = WSA_INVALID_EVENT;

    bool m_isConnected = false;
};

// 소켓 클수는 네트워크 작업을 위한 래퍼
// 데이터그램이나 스트림에 상관없이 다 제공해주고 있어야함
// 패킷 종류에 따라 관리해야할 패킷 컨테이너가 달라집니다.
// 스트림이라면 버퍼에 계속 읽은 데이터를 누적하는거고 (TCP)
// 데이터그램이라면 각 패킷은 분리되어 저장됩니다. (UDP)
//

int main()
{
    WinSockClient gClient;  // 연결을 위한 객체를 만들어서
    
    // 윈속 초기화 하고
    WSAData wsaData;

    if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return 0;

    gClient.Start(true);

    if (false == gClient.Connect())
    {
        cout << "Connect Error " << WSAGetLastError() << endl;
    }

    while (true)
    {

    }

    gClient.CloseSocket();
    WSACleanup();

    return 0;

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
    inet_pton(AF_INET, "172.21.1.68", &(serverAddr.sin_addr));  // 여기 주소 바꾸기

    if (::connect(m_clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) return false;

    return true;
}

void WinSockClient::CloseSocket()
{
    closesocket(m_clientSocket);
}