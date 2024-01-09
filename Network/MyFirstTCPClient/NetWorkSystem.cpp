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

void NetWorkSystem::PostMsg(char* str, const int size)
{
    PacketC2S_BroadcastMsg inputMsg;
    inputMsg.id = C2S_BROADCAST_MSG;
    inputMsg.size = size;
    inputMsg.clientMessage = str + '\0';

    m_sendBuffer[0] = inputMsg.size / 10 + '0';
    m_sendBuffer[1] = inputMsg.size % 10 + '0';
    m_sendBuffer[2] = inputMsg.id / 10 + '0';
    m_sendBuffer[3] = inputMsg.id % 10 + '0';

    memcpy(m_sendBuffer + 4, inputMsg.clientMessage, size);

    m_sendBytes += size;
}

// �������� ���� �޽����� Ŭ�� Pop���ִ� �Լ�
PacketS2C_BroadcastMsg* NetWorkSystem::PopMsg()
{
    if (m_recvBytes < 1)
    {
        return nullptr;
    }
    // todo : ������ȭ �ٲٱ�
    PacketS2C_BroadcastMsg msg;
    msg.size = static_cast<short>(m_recvBuffer[0]) * 10 + static_cast<short>(m_recvBuffer[1]) / 10;
    msg.id = static_cast<EPacketId>((m_recvBuffer[2] - '0') * 10 + (m_recvBuffer[3] - '0'));;
    msg.serverMessage = m_recvBuffer + 4;

    if (msg.size != m_recvBytes)
        return nullptr;
    if (msg.id != S2C_BROADCAST_MSG)
        return nullptr;

    m_recvBytes -= msg.size;

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
        OnReceive();
    }
}

void NetWorkSystem::NetUpdate()
{
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
            // 
        }
    }
    // popmsg�� ����.
    /*else if (m_recvBytes > 1024)
    {
        memcpy(m_sendBuffer, m_recvBuffer, m_recvBytes);
        m_recvBytes = 0;
        
    }*/
}

void NetWorkSystem::OnReceive()
{
    char* recvBuffer = new char[RCV_BUF_SIZE];
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

    memcpy(m_recvBuffer + m_recvBytes, recvBuffer, recvBytes);

    m_recvBytes += recvBytes;
}
