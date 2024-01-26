#include "pch.h"
#include "Client.h"
#include "WorldManager.h"

#include "../NetworkLibrary/ClientSocket.h"
#include "../NetworkLibrary/WinSock.h"

void ClientNetworkManager::Start()
{
    WSAData wsaData;

    if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return;

    m_clientSocket = std::make_shared<ClientSocket>();
    if (!m_clientSocket->Create())
        return;

    m_clientSocket->Connect("172.21.1.81", 7777);
}

void ClientNetworkManager::Update()
{
    WSAEVENT clientEvent = m_clientSocket->GetEvent();
    int index = ::WSAWaitForMultipleEvents(1, &clientEvent, FALSE, 1, FALSE);

    if (index == WSA_WAIT_FAILED)
        return;

    index -= WSA_WAIT_EVENT_0;

    if (index == WSA_WAIT_TIMEOUT)
        return;

    WSANETWORKEVENTS networkEvents;
    if (::WSAEnumNetworkEvents(m_clientSocket->GetHandle(), clientEvent, &networkEvents) == SOCKET_ERROR)
        return;

    if (networkEvents.lNetworkEvents & FD_CONNECT)
    {
        if (networkEvents.iErrorCode[FD_CONNECT_BIT] != 0)
        {
            OnNetError(networkEvents.iErrorCode[FD_WRITE_BIT], "Connect", m_clientSocket);
            return;
        }
        return;
    }

    if (networkEvents.lNetworkEvents & FD_READ)
    {
        if (networkEvents.iErrorCode[FD_READ_BIT] != 0)
        {
            OnNetError(networkEvents.iErrorCode[FD_WRITE_BIT], "Read", m_clientSocket);
            return;
        }
        OnReceive();
    }

    if (networkEvents.lNetworkEvents & FD_WRITE)
    {
        if (networkEvents.iErrorCode[FD_WRITE_BIT] != 0)
        {
            OnNetError(networkEvents.iErrorCode[FD_WRITE_BIT], "Write", m_clientSocket);
            return;
        }

        OnSend();
    }

    if (networkEvents.lNetworkEvents & FD_CLOSE)
    {
        if (networkEvents.iErrorCode[FD_CLOSE_BIT] != 0)
        {
            return;
        }
    }
}

void ClientNetworkManager::NetUpdate()
{
    // todo 채원: RecvQueue는 WorldManager에서 관리하는 것이 좋을듯, 그리고 World 바뀔 때마다 queue 초기화
   /* while(!WorldManager::GetInstance()->IsRecvQueueEmpty())
    {
        Read(WorldManager::GetInstance()->GetRecvQueueFront(), strlen(WorldManager::GetInstance()->GetRecvQueueFront()));
    }*/

    while(!WorldManager::GetInstance()->IsSendQueueEmpty())
    {
        int nSent = m_clientSocket->Send(WorldManager::GetInstance()->GetSendQueueFrontBuffer(), WorldManager::GetInstance()->GetSendQueueFrontLen());

        short size = static_cast<short>(WorldManager::GetInstance()->GetSendQueueFrontBuffer()[0] - '0') * 10 + static_cast<short>(WorldManager::GetInstance()->GetSendQueueFrontBuffer()[1] - '0');

        // todo 채원: 사이즈가 다를 때 해주는거 바꾸기
        if (size != nSent)
            continue;

        if (nSent > 0)
        {
            char* tmp = WorldManager::GetInstance()->GetSendQueueFrontBuffer();
            WorldManager::GetInstance()->PopSendQueue();
            delete[] tmp;
        }
        else if (nSent == 0)
        {
            // send 할 수 없는 상태인지 아닌지 변수 만들기?
            // 근데 이렇게 되면 서버가 언제 준비되는지 모름.
        }
    }
}

void ClientNetworkManager::OnReceive()
{
    char* recvMsg = new char[RCV_BUF_SIZE];
    int recvBytes = m_clientSocket->Recv(recvMsg, RCV_BUF_SIZE);

    if (recvBytes == SOCKET_ERROR)
    {
        return;
    }
    // 읽을거 없을 때
    if (recvBytes == 0)
    {
        return;
    }
    
    // todo 채원: Recv 됐으니까 여기에 WorldManager을 통해서 queue에 데이터 넣어주기. 
    printf("readBuffer: %s \n", recvMsg);
    WorldManager::GetInstance()->PushRecvQueue(recvMsg, recvBytes);
}

void ClientNetworkManager::OnSend()
{

}

void ClientNetworkManager::OnClose()
{
}

void ClientNetworkManager::Stop()
{
    ::WSACleanup();
}

void ClientNetworkManager::Read(char buf[], int size)
{
    int id = static_cast<int>((buf[0] - '0') * 10 + (buf[1] - '0'));

    if (id == 20)
    {
        m_bReadyState = true;
    }
}

void ClientNetworkManager::Write(char buf[], int size)
{
}

void ClientNetworkManager::OnNetError(int errorCode, const char* errorMsg, std::shared_ptr<WinSock> pSocket)
{
    if (errorMsg)
    {
        printf("\n onNetError %s \t", errorMsg);
    }

    if (pSocket)
    {
        printf("\n onNetError  %s : %d", pSocket->GetIP().c_str(), pSocket->GetPort());
    }

    printf("NetErrorCode  %d  \n", errorCode);
}