#include "Session.h"
#include "Client.h"

#include <cassert>

constexpr int SND_BUF_SIZE = 1024;
constexpr int RCV_BUF_SIZE = 9604;

namespace netfish
{
	SessionId Session::s_sessionIdCounter;

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

	void Session::Write(char* pData, int len)
	{
		PacketS2C_BroadcastMsg* inputMsg = new PacketS2C_BroadcastMsg;
		inputMsg->id = S2C_BROADCAST_MSG;
		inputMsg->size = len + 5;
		inputMsg->serverMessage = pData + '\0';

		m_writeBuffer[0] = inputMsg->size / 10 + '0';
		m_writeBuffer[1] = inputMsg->size % 10 + '0';
		m_writeBuffer[2] = inputMsg->id / 10 + '0';
		m_writeBuffer[3] = inputMsg->id % 10 + '0';

		memcpy(m_writeBuffer + 4, inputMsg->serverMessage, len + 1);

		m_writeBytes += len + 5;
	}

	PacketC2S_BroadcastMsg* Session::Read(char* pData, int len)
	{
		// 여기는 오해가 있으니 감춘다.
		// 소켓에 m_readBuffer를 던져서 읽어오게 한다.
		// TODO: 받은 패킷을 어떻게 파싱할 것인가에 따라 구조가 바뀌므로 일단은 나중에 다시.
		//

		PacketC2S_BroadcastMsg msg;
		msg.size = static_cast<short>(m_readBuffer[0] - '0') * 10 + static_cast<short>(m_readBuffer[1] - '0');
		msg.id = static_cast<EPacketId>((m_readBuffer[2] - '0') * 10 + (m_readBuffer[3] - '0'));
		msg.clientMessage = m_readBuffer + 4 + '\0';

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
			msg = Read(m_readBuffer, strlen(m_readBuffer));
			
			if (msg == nullptr)
				return;

			// 클라에서 보낸 메시지가 abc일 때
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
			// 클라에 보낼 메시지 m_writeBuffer에 복사

			int nSent = m_pClient->Send(m_writeBuffer, m_writeBytes);

			if (nSent > 0)
			{
				m_writeBytes -= nSent;

				if (m_writeBytes > 0)
				{
					memmove(m_writeBuffer, m_writeBuffer + nSent, m_writeBytes);
				}
			}

			// 소켓 버퍼가 가득 차서 전송이 불가능
			else if (nSent == 0)
			{

			}

			// 소켓 에러가 발생함. 우짤까
			else
			{

			}			
		}		
	}

	void Session::ReadUpdate()
	{
		// 소켓에 m_readBuffer를 던져서 읽어오게 한다.
		// todo : 받은 패킷을 어떻게 파싱할 것인가

		assert(m_pClient != nullptr);

		int buflen = BUF_SIZE - m_readBytes;

		int nRead = m_pClient->Recv(m_readBuffer, RCV_BUF_SIZE);

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