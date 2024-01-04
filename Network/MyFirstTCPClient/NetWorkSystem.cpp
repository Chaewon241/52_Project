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

void NetWorkSystem::Initialize()
{
    WSAData wsaData;

    if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return;

    // m_sendBuffer 와 m_recvBuffer 를 동적 할당, 나중에 해제 필수
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
    m_sendBuffer[0] = pMsg->size / 10 + '0';
    m_sendBuffer[1] = pMsg->size % 10 + '0';
    m_sendBuffer[2] = pMsg->id / 10 + '0';
    m_sendBuffer[3] = pMsg->id % 10 + '0';

    memcpy(m_sendBuffer + 4, pMsg->clientMessage, size);

    m_sendBytes += size;
}

// 서버에서 받은 메시지를 클라에 Pop해주는 함수
PacketS2C_BroadcastMsg* NetWorkSystem::PopMsg()
{
    if (m_recvBytes < 1)
    {
        return nullptr;
    }
    // todo : 역직렬화 바꾸기
    PacketS2C_BroadcastMsg* msg = new PacketS2C_BroadcastMsg;
    msg->size = static_cast<short>(m_recvBuffer[0]) * 10 + static_cast<short>(m_recvBuffer[1]) / 10;
    msg->id = S2C_BROADCAST_MSG;
    msg->serverMessage = m_recvBuffer + 4;

    m_recvBytes = 0;

    return msg;
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
        // 이 경우 기다렸다가 다시 시도
        else if (nSent == 0)
        {
            // send 할 수 없는 상태인지 아닌지 변수 만들기?
            // 
        }
    }
    // popmsg에 있음.
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

    // 읽을거 없을 때
    if (recvBytes == 0)
    {
        cout << "Disconnected" << endl;
        return;
    }

    memcpy(m_recvBuffer + m_recvBytes, recvBuffer, recvBytes);

    m_recvBytes += recvBytes;
}
