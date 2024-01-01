#include "WinSockClient.h"
#include "WinSock.h"
#include "NetWorkSystem.h"
#include "../MyFirstTcpServer/Types.h"

#pragma comment (lib, "Ws2_32.lib")

using namespace std;
using namespace netfish;

// 소켓 클수는 네트워크 작업을 위한 래퍼
// 데이터그램이나 스트림에 상관없이 다 제공해주고 있어야함
// 패킷 종류에 따라 관리해야할 패킷 컨테이너가 달라집니다.
// 스트림이라면 버퍼에 계속 읽은 데이터를 누적하는거고 (TCP)
// 데이터그램이라면 각 패킷은 분리되어 저장됩니다. (UDP)
//

int main()
{
    // 초기화
    NetWorkSystem::GetNetWorkSystemInstance()->Initialize();

    while (true)
    {
        // 입력 받은 값이 있으면 해당 함수로 이동 -> inputSystem을 통해서 다시 하기
        if (GetAsyncKeyState(0x41) & 0x0001)
        {
            char* str;
            scanf("%c", str);
            NetWorkSystem::GetNetWorkSystemInstance();
        }
        // NetworkSystem에서 DoUpdate 가져오기
        NetWorkSystem::GetNetWorkSystemInstance()->DoUpdate();

        NetMsg* pMsg = nullptr;
        while (pMsg = NetWorkSystem::GetNetWorkSystemInstance()->PopMsg())
        {
            // pMsg를 처리하는 함수
        }
    }

    Client.CloseSocket();
    WSACleanup();

    return 0;

}
