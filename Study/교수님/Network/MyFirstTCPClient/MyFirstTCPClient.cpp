#include "WinSockClient.h"
#include "NetworkSystem.h"


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
    WinSockClient Client;
    
    // 윈속 초기화 하고
    WSAData wsaData;

    if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return 0;

    Client.Initialize();

    if (false == Client.Connect())
    {
        cout << "Connect Error " << WSAGetLastError() << endl;
    }

    while (true)
    {
        //Client.DoUpdate();

        NetworkSystem::DoUpate();

        struct NetMsg
        {
            SessionId id;
            char* pBuf; // 동적할당된 메모리 주소: 나중에 패킷 풀을 만들면 그 때 교체
        };

        NetMsg* pMsg = nullptr;
        while (pMsg = NetworkSystem::PopMsg())
        {
            //  수신한 메시지가 있다면 여기서 처리하는 함수 쪽으로 보낸다.
            // 처리할 객체로 전달하는 디스패치를 수행해야 한다.


        }


        // 마우스 이벤트나 키보드 입력을 받으면 InputSystem 를 업데이트 했음
        // 키 값이 A, B 인 경우를 분리해 본다.
        // 해당 키 값에 따라서 네트워크로 보내는 경우와  아닌 경우가 있을 수 있다.
        // 통신이 필요한 입력이라면 패킷을 만들어서 서버로 송신 합니다.
        // InputSystem 을 붙이고 패킷을 보내는 구조. Client.Send() 를 하게 되겠지?
        
        if (GetAsyncKeyState(VK_LEFT) & 0x0001)
        {
            Client.PressKey();
        }
    }

    Client.CloseSocket();
    WSACleanup();

    return 0;

}
