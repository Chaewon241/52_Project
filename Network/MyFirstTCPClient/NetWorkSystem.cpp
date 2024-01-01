#include "NetWorkSystem.h"
#include "WinSockClient.h"

using namespace std;

// 여기서 send, recv, connect 다 하는거고
// 그 값들을 클라이언트에 복사시킴
//

NetWorkSystem* NetWorkSystem::m_NetWorkSystemInstance = nullptr;

constexpr int SND_BUF_SIZE = 1024;
constexpr int RCV_BUF_SIZE = 9604;

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

    delete m_Client;
    m_Client = nullptr;

    delete m_sendBuffer;
    m_sendBuffer = nullptr;

    delete m_recvBuffer;
    m_recvBuffer = nullptr;
}

void NetWorkSystem::PostMsg(netfish::NetMsg* pMsg, const int size)
{
    m_sendBuffer = pMsg->pBuf;
    m_Client->WriteSendBuffer(m_sendBuffer, size);
}

netfish::NetMsg* NetWorkSystem::PopMsg()
{
    netfish::NetMsg nm;
    nm.id = 1;
    nm.pBuf = m_recvBuffer;

    return &nm;
}

void NetWorkSystem::Initialize()
{
    WSAData wsaData;

    if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return;

    m_Client = new WinSockClient;

    m_Client->Initialize();

    if (false == m_Client->Connect())
    {
        cout << "Connect Error " << WSAGetLastError() << endl;
    }
}

void NetWorkSystem::DoUpdate()
{
    int eventState = m_Client->EventState();

    if (eventState == WSA_WAIT_FAILED)
        return;

    eventState -= WSA_WAIT_EVENT_0;

    WSANETWORKEVENTS networkEvents;
    if (::WSAEnumNetworkEvents(m_Client->GetSocket(), m_Client->GetWSAEvent()[eventState], &networkEvents) == SOCKET_ERROR)
        return;

    if (networkEvents.lNetworkEvents & FD_CONNECT)
    {
        if (networkEvents.iErrorCode[FD_CONNECT_BIT] != 0)
        {
            m_bConnected = false;
            return;
        }
        m_bConnected = true;
    }

    if (networkEvents.lNetworkEvents & FD_READ)
    {
        if (networkEvents.iErrorCode[FD_READ_BIT] != 0)
        {
            cout << "Read Error" << endl;
            return;
        }

        // 랩핑해야할듯
        int recvBytes = ::recv(m_Client->GetSocket(), m_recvBuffer, RCV_BUF_SIZE, 0);

        if (recvBytes == SOCKET_ERROR)
        {
            cout << "Read Error" << endl;
            return;
        }

        // 읽을거 없을 때
        if (recvBytes == 0)
        {
            cout << "Disconnected" << endl;
            return;
        }
        cout << "Recv: " << m_recvBuffer << endl;

        // recv한 데이터를 client에 넘겨주기.
        m_Client->RecvReadBuffer(m_recvBuffer);
    }

    // 클라이언트 세션은 일단 하나로
    m_Client->NetUpdate();
}
