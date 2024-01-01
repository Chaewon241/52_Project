#include "NetWorkSystem.h"
#include "WinSockClient.h"

using namespace std;

// ���⼭ send, recv, connect �� �ϴ°Ű�
// �� ������ Ŭ���̾�Ʈ�� �����Ŵ
//

NetWorkSystem* NetWorkSystem::m_NetWorkSystemInstance = nullptr;

constexpr int SND_BUF_SIZE = 1024;
constexpr int RCV_BUF_SIZE = 9604;

// �̱������� ��������
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

        // �����ؾ��ҵ�
        int recvBytes = ::recv(m_Client->GetSocket(), m_recvBuffer, RCV_BUF_SIZE, 0);

        if (recvBytes == SOCKET_ERROR)
        {
            cout << "Read Error" << endl;
            return;
        }

        // ������ ���� ��
        if (recvBytes == 0)
        {
            cout << "Disconnected" << endl;
            return;
        }
        cout << "Recv: " << m_recvBuffer << endl;

        // recv�� �����͸� client�� �Ѱ��ֱ�.
        m_Client->RecvReadBuffer(m_recvBuffer);
    }

    // Ŭ���̾�Ʈ ������ �ϴ� �ϳ���
    m_Client->NetUpdate();
}
