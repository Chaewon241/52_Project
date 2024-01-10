#include "WinSockClient.h"
#include "WinSock.h"
#include "NetWorkSystem.h"
#include "InputSystem.h"
#include "MyProtocol.h"

#pragma comment (lib, "Ws2_32.lib")

using namespace std;

int main()
{
    WinSockClient* mClient = new WinSockClient;

    // 윈속 초기화
    NetWorkSystem::GetNetWorkSystemInstance()->Initialize();

    mClient->Initialize();
    NetWorkSystem::GetNetWorkSystemInstance()->SetClient(mClient);

    if (false == mClient->Connect())
    {
        cout << "Connect Error " << WSAGetLastError() << endl;
    }

    while (true)
    {
        InputSystem inputSys;

        if (inputSys.isKeyPressed('A'))
        {
            char* str = new char[100];
            std::cin.getline(str, 100);

            NetWorkSystem::GetNetWorkSystemInstance()->PostMsg(str, strlen(str));
        }
        // NetworkSystem에서 DoUpdate 가져오기
        NetWorkSystem::GetNetWorkSystemInstance()->DoUpdate();
        NetWorkSystem::GetNetWorkSystemInstance()->NetUpdate();

        PacketS2C_BroadcastMsg* serverMsg = nullptr;

        while (serverMsg = NetWorkSystem::GetNetWorkSystemInstance()->PopMsg())
        {
            // serverMsg를 처리하는 함수
            cout << "server to client recv data: " << serverMsg->serverMessage << endl;
        }
    }

    mClient->CloseSocket();
    WSACleanup();
    delete mClient;
    return 0;

}
