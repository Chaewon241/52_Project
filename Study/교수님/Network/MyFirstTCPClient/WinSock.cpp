#include "WinSockClient.h"
#include "WinSock.h"
#include <iostream>

using namespace std;

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

bool WinSock::Connect(UINT nSocketPort, const char* nIP)
{
    SOCKADDR_IN serverAddr;
    ::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = ::htons(7777);
    inet_pton(AF_INET, nIP, &(serverAddr.sin_addr));  // 여기 주소 바꾸기

    if (::connect(m_hSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) return false;

    return true;
}

bool WinSock::DisConnect()
{
    int iResult = shutdown(m_hSocket, SD_SEND);
    if (iResult == SOCKET_ERROR)
    {
        std::cout << "DisConnect Error" << WSAGetLastError() << endl;
        return false;
    }
    return true;
}

bool WinSock::CreateEventHandle()
{
    // 이벤트 생성해주고
    m_WSAEvent = ::WSACreateEvent();
    
    // 소켓에 이벤트 설정
    if (::WSAEventSelect(m_hSocket, m_WSAEvent, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR) return false;
    return true;
}

void MyWinSocket::OnConnect(int nErrorCode)
{
  
}

void MyWinSocket::OnSend(int nErrorCode)
{
    //if (nErrorCode != 0)
    //    return;

    //Session& s = sessions[index];

    //// Receive
    //if (s.recvBytes == 0)
    //{
    //    int recvLen = ::recv(s.socket, s.recvBuffer, BUFSIZE, 0);
    //    if (recvLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK) {
    //        return false;
    //    }

    //    if (recvLen > 0)
    //        s.recvBytes = recvLen;

    //    cout << "Recv Data= " << recvLen << " : " << s.recvBuffer << endl;
    //}

    //// Send
    //if (s.recvBytes > s.sendBytes) {
    //    int sendLen = ::send(s.socket, &s.recvBuffer[s.sendBytes], s.recvBytes - s.sendBytes, 0);
    //    if (sendLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK) {
    //        return false;
    //    }

    //    s.sendBytes += sendLen;
    //    if (s.recvBytes == s.sendBytes) {
    //        s.recvBytes = 0;
    //        s.sendBytes = 0;
    //    }

    //    cout << "Send Data= " << sendLen << endl;
    //}
}
