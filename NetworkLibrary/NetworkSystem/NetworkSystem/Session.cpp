#include "Session.h"
#include "ClientSocket.h"

Session::Session()
{
	m_writeBuffer = new char[SND_BUF_SIZE];
	m_readBuffer = new char[RCV_BUF_SIZE];
}

Session::~Session()
{
	delete m_writeBuffer;
	delete m_readBuffer;
}

void Session::NetUpdate()
{
	PacketC2S_BroadcastMsg* msg = new PacketC2S_BroadcastMsg;
	if (m_readBytes > 0)
	{
		// Read
		msg = Read(m_readBuffer, strlen(m_readBuffer));

		if (msg == nullptr)
			return;

		// Ŭ�󿡼� ���� �޽����� abc�� ��
		if (strcmp(msg->clientMessage, "abcd") == 0)
		{
			char destination[20];
			const char* source = "Right";

			strcpy_s(destination, sizeof(destination), source);

			Write(destination, strlen(source));
		}
		else
		{
			char destination[20];
			const char* source = "Wrong";

			strcpy_s(destination, sizeof(destination), source);

			Write(destination, strlen(source));
		}
	}

	if (m_writeBytes > 0)
	{
		// Ŭ�� ���� �޽��� m_writeBuffer�� ����

		int nSent = m_pClient->Send(m_writeBuffer, m_writeBytes);

		if (nSent > 0)
		{
			m_writeBytes -= nSent;

			if (m_writeBytes > 0)
			{
				memmove(m_writeBuffer, m_writeBuffer + nSent, m_writeBytes);
			}
		}

		// ���� ���۰� ���� ���� ������ �Ұ���
		else if (nSent == 0)
		{

		}

		// ���� ������ �߻���. ��©��
		else
		{

		}
	}
}

void Session::ReadUpdate()
{
	int buflen = BUF_SIZE - m_readBytes;

	int nRead = m_pClient->Recv(m_readBuffer, RCV_BUF_SIZE);

	if (nRead == -1)
	{
		return;
	}

	m_readBytes += nRead;
}

void Session::SetClient(ClientSocket* pClient)
{
	m_sessionId = s_sessionIdCounter++;

	m_pClient = pClient;

	m_pClient->SetSessionId(m_sessionId);
}
