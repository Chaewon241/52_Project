#include "WinSockBase.h"

bool WinsockInit()
{
    return false;
}

void WinsockCleanUp()
{
}

WinSockBase::WinSockBase(void)
{
}

WinSockBase::~WinSockBase(void)
{
}

// ������ ����, Ŭ���̾�Ʈ ����, 
bool WinSockClient::Connect()
{
    m_clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);

    if (m_clientSocket == INVALID_SOCKET) return false;

    m_clientEvent = ::WSACreateEvent();

    if (::WSAEventSelect(m_clientSocket, m_clientEvent, FD_CONNECT | FD_READ | FD_WRITE | FD_CLOSE) == SOCKET_ERROR) return 0;

    SOCKADDR_IN serverAddr;
    ::memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = ::htons(7777);
    inet_pton(AF_INET, "172.21.1.81", &(serverAddr.sin_addr));  // ���� �ּ� �ٲٱ�

    if (::connect(m_clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) return false;

    return true;
}

bool WinSockClient::DoUpdate()
{
    // �̺�Ʈ ����
    // �̺�Ʈ��ü�� �ϳ��� ���� ������ ���°� �Ǿ����� Ȯ��.
    int index = ::WSAWaitForMultipleEvents(1, &m_clientEvent, FALSE, 1000, FALSE);
    // ���� ������ ������ �̺�Ʈ �� ���� ���� �߰ߵ� �̺�Ʈ�� �ε����� ��ȯ
    if (index == WSA_WAIT_FAILED)
        return false;

    index -= WSA_WAIT_EVENT_0;

    // ������ ���� ����
    WSANETWORKEVENTS networkEvents;

    // WSAEnumNetworkEvents �Լ��� ���Ͽ� �߻��� ��Ʈ��ũ �̺�Ʈ�� Ȯ���ϴ� �� ���
    // �ַ� WSAEvent�� �Բ� ���Ǿ� ���Ͽ��� �߻��� �̺�Ʈ�� �ִ��� Ȯ���ϰ� 
    // �̺�Ʈ�� ���¸� ��� �� Ȱ��
    if (::WSAEnumNetworkEvents(m_clientSocket, m_clientEvent, &networkEvents) == SOCKET_ERROR)
    {
        cout << "EnumNetworkEvents Error" << WSAGetLastError() << endl;
        return false;
    }

    // ������� ����: �̺�Ʈ ��ü ���� Ȯ��(WSAWaitForMultipleEvents)
    // -> ���Ͽ��� �߻��� ��Ʈ��ũ �̺�Ʈ�� ���� ���� ���(WSAEnumNetworkEvents)

    // ���࿡ Ŀ��Ʈ���
    if (networkEvents.lNetworkEvents & FD_CONNECT)
    {
        if (networkEvents.iErrorCode[FD_CONNECT_BIT] != 0)
        {
            cout << "Connect Error " << networkEvents.iErrorCode[FD_CONNECT_BIT] << endl;
            return false;
        }

        cout << "Connected" << endl;

        m_isConnected = true;
    }

    // receive ���� �о�ð� �ִٸ�
    if (networkEvents.lNetworkEvents & FD_READ)
    {
        if (networkEvents.iErrorCode[FD_READ_BIT] != 0)
        {
            cout << "Read Error" << endl;
            return false;
        }
        char buffer[BUFSIZE] = {};
        int recvBytes = ::recv(m_clientSocket, buffer, BUFSIZE, 0);
        if (recvBytes == SOCKET_ERROR)
        {
            cout << "Read Error" << endl;
            return false;
        }

        // ������ ���� ��
        if (recvBytes == 0)
        {
            cout << "Disconnected" << endl;
            return false;
        }
        cout << "Recv: " << buffer << endl;
    }

    if (m_isConnected)
    {
        Sleep(1000);

        int sendLen = ::send(m_clientSocket, m_sendBuffer, strlen(m_sendBuffer) + 1, 0);
        if (sendLen == SOCKET_ERROR && ::WSAGetLastError() != WSAEWOULDBLOCK) {
            cout << "send Error " << ::WSAGetLastError() << endl;
        }
        else
        {
            cout << "send " << sendLen << " " << endl;
        }
    }
    return true;
}

void WinSockClient::CloseSocket()
{
    closesocket(m_clientSocket);
}

