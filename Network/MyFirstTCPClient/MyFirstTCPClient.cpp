
#include "WinSockBase.h"
#include "../MyFirstTcpServer/WinSock.h"

#pragma comment (lib, "Ws2_32.lib")

using namespace std;

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

    if (false == gClient.Connect())
    {
        cout << "Connect Error " << WSAGetLastError() << endl;
    }

    while (true)
    {
        gClient.MainLoop();
    }

    gClient.CloseSocket();
    WSACleanup();

    return 0;

}
