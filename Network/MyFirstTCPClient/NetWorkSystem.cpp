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

void NetWorkSystem::Initialize()
{
    WSAData wsaData;

    if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return;

    // m_sendBuffer �� m_recvBuffer �� ���� �Ҵ�, ���߿� ���� �ʼ�
    m_sendBuffer = new char[SND_BUF_SIZE + 1];
    m_recvBuffer = new char[RCV_BUF_SIZE];

    m_sendBuffer[SND_BUF_SIZE] = '\0';
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

void NetWorkSystem::PostMsg(PacketC2S_BroadcastMsg* pMsg, const int size)
{
    memcpy(m_sendBuffer, pMsg, size);

    m_sendBytes += size;

    // event �߻��� ���ؼ� ����ٰ� �ϴ� �����ϴ�.
    if (m_sendBytes)
    {
        int nSent = m_Client->Send(m_sendBuffer, m_sendBytes);
        if (nSent > 0)
        {
            m_sendBytes -= nSent;

            if (m_sendBytes > 0)
            {
                memmove(m_sendBuffer, m_sendBuffer + nSent, m_sendBytes);
            }
        }
        // �� ��� ��ٷȴٰ� �ٽ� �õ�
        else if (nSent == 0)
        {
            // send �� �� ���� �������� �ƴ��� ���� �����?
            // �ٵ� �̷��� �Ǹ� ������ ���� �غ�Ǵ��� ��.
        }
    }
}

// �������� ���� �޽����� Ŭ�� Pop���ִ� �Լ�
PacketS2C_BroadcastMsg* NetWorkSystem::PopMsg()
{
    if (m_recvBytes < 1024)
    {
        return nullptr;
    }
    // todo : ������ȭ �ٲٱ�
    PacketS2C_BroadcastMsg msg;
    memcpy(&msg, m_recvBuffer, sizeof(PacketS2C_BroadcastMsg));

    return &msg;
}

void NetWorkSystem::SetClient(WinSockClient* Client)
{
    m_Client = Client;
}

void NetWorkSystem::DoUpdate()
{
    int eventState = m_Client->EventState();

    if (eventState == WSA_WAIT_FAILED)
        return;

    eventState -= WSA_WAIT_EVENT_0;

    if (eventState == 258)
        return;

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
        char* recvBuffer = nullptr;
        int recvBytes = ::recv(m_Client->GetSocket(), recvBuffer, RCV_BUF_SIZE, 0);

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

        if (recvBuffer)
            memcpy(m_recvBuffer + strlen(m_recvBuffer), recvBuffer, recvBytes);


        m_recvBytes += recvBytes;

        // recv�� �����͸� client�� �Ѱ��ֱ�.
        //m_Client->RecvReadBuffer(m_recvBuffer);
    }

    // Ŭ���̾�Ʈ ������ �ϴ� �ϳ���
    // NetUpdate�� NetworkSystem���� ����߰ڴ�.
    //m_Client->NetUpdate();

    NetUpdate();
}

void NetWorkSystem::NetUpdate()
{
    //if (m_sendBytes > 1024)
    //{
    //    int nSent = m_Client->Send(m_sendBuffer, m_sendBytes);
    //    if (nSent > 0)
    //    {
    //        m_sendBytes -= nSent;

    //        if (m_sendBytes > 0)
    //        {
    //            memmove(m_sendBuffer, m_sendBuffer + nSent, m_sendBytes);
    //        }
    //    }
    //    // �� ��� ��ٷȴٰ� �ٽ� �õ�
    //    else if (nSent == 0)
    //    {
    //        // send �� �� ���� �������� �ƴ��� ���� �����?
    //        // �ٵ� �̷��� �Ǹ� ������ ���� �غ�Ǵ��� ��.
    //    }
    //}
    // popmsg�� ����.
    /*else if (m_recvBytes > 1024)
    {
        memcpy(m_sendBuffer, m_recvBuffer, m_recvBytes);
        m_recvBytes = 0;
        
    }*/
}
