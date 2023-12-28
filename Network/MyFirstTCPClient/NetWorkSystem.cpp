#include "NetWorkSystem.h"
#include "WinSockClient.h"

using namespace std;

NetWorkSystem* NetWorkSystem::m_NetWorkSystemInstance = nullptr;

// 싱글턴으로 만들어놓기
NetWorkSystem* NetWorkSystem::GetNetWorkSystemInstance()
{
    if (m_NetWorkSystemInstance == nullptr)
    {
        m_NetWorkSystemInstance = new NetWorkSystem();
    }

    return m_NetWorkSystemInstance;
}

void NetWorkSystem::DestroyInstance()
{
    delete m_NetWorkSystemInstance;
    m_NetWorkSystemInstance = nullptr;
}

void NetWorkSystem::Initialize()
{
    m_Client = new WinSockClient;
}

void NetWorkSystem::DoUpdate(SOCKET socket, int index)
{
    WSANETWORKEVENTS networkEvents;
    if (::WSAEnumNetworkEvents(socket, m_Client->GetWSAEvent()[index], &networkEvents) == SOCKET_ERROR)
        return;

    if (networkEvents.lNetworkEvents & FD_CONNECT)
    {
        if (networkEvents.iErrorCode[FD_CONNECT_BIT] != 0)
            return;
        m_Client->OnConnect(networkEvents.iErrorCode[FD_CONNECT_BIT]);
    }

    if (networkEvents.lNetworkEvents & FD_READ)
    {
        if (networkEvents.iErrorCode[FD_READ_BIT] != 0)
        {
            cout << "Read Error" << endl;
            return;
        }
        m_Client->OnReceive(networkEvents.iErrorCode[FD_READ_BIT]);
    }
}
