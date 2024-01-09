#include "Session.h"
#include "Client.h"

#include <cassert>

namespace netfish
{
	SessionId Session::s_sessionIdCounter;

	Session::Session()
	{

	}

	Session::~Session()
	{
	}

	void Session::Write(char* pData, int len)
	{
		PacketS2C_BroadcastMsg* inputMsg = new PacketS2C_BroadcastMsg;
		inputMsg->id = S2C_BROADCAST_MSG;
		inputMsg->size = sizeof(PacketS2C_BroadcastMsg);
		inputMsg->serverMessage = pData;

		m_writeBuffer[0] = inputMsg->size / 10 + '0';
		m_writeBuffer[1] = inputMsg->size % 10 + '0';
		m_writeBuffer[2] = inputMsg->id / 10 + '0';
		m_writeBuffer[3] = inputMsg->id % 10 + '0';

		memcpy(m_writeBuffer + 4, inputMsg->serverMessage, len);

		m_writeBytes += len;
	}

	PacketC2S_BroadcastMsg* Session::Read(char* pData, int len)
	{
		// ����� ���ذ� ������ �����.
		// ���Ͽ� m_readBuffer�� ������ �о���� �Ѵ�.
		// TODO: ���� ��Ŷ�� ��� �Ľ��� ���ΰ��� ���� ������ �ٲ�Ƿ� �ϴ��� ���߿� �ٽ�.
		//

		PacketC2S_BroadcastMsg msg;
		msg.size = static_cast<short>(m_readBuffer[0] - '0') * 10 + static_cast<short>(m_readBuffer[1] - '0');
		msg.id = static_cast<EPacketId>((m_readBuffer[2] - '0') * 10 + (m_readBuffer[3] - '0'));
		msg.clientMessage = m_readBuffer + 4;

		if (msg.size != m_readBytes)
			return nullptr;
		if (msg.id != C2S_BROADCAST_MSG)
			return nullptr;

		m_readBytes -= msg.size;

		return &msg;
	}

	void Session::NetUpdate()
	{
		PacketC2S_BroadcastMsg* msg = new PacketC2S_BroadcastMsg;
		if (m_readBytes > 0)
		{
			// Read
			msg = Read(m_readBuffer, m_readBytes);
			
			if (msg == nullptr)
				return;

			// Ŭ�󿡼� ���� �޽����� abc�� ��
			if (msg->clientMessage == "abc")
			{
				char destination[20];
				const char* source = "Right";

				strcpy_s(destination, sizeof(destination), source);

				Write(destination, sizeof(PacketS2C_BroadcastMsg));
			}
			else
			{
				char destination[20];
				const char* source = "Wrong";

				strcpy_s(destination, sizeof(destination), source);

				Write(destination, sizeof(PacketS2C_BroadcastMsg));
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
		// ���Ͽ� m_readBuffer�� ������ �о���� �Ѵ�.
		// todo : ���� ��Ŷ�� ��� �Ľ��� ���ΰ�

		assert(m_pClient != nullptr);

		int buflen = BUF_SIZE - m_readBytes;

		int nRead = m_pClient->Recv(m_readBuffer, buflen);

		if (nRead == -1)
		{
			return;
		}

		m_readBytes += nRead;
	}

	void Session::SetClient(Client* pClient)
	{
		m_sessionId = ++s_sessionIdCounter;

		m_pClient = pClient;

		m_pClient->SetSessionId(m_sessionId);
	}
}