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

	void Session::Write(const char* pData, int len)
	{
		// m_pClient->Send를 바로 호출하지 않고 m_writeBuffer에 복사해둔다.
		//
	}

	void Session::Read(char* pData, int len)
	{
		// 여기는 오해가 있으니 감춘다.
		// 소켓에 m_readBuffer를 던져서 읽어오게 한다.
		// TODO: 받은 패킷을 어떻게 파싱할 것인가에 따라 구조가 바뀌므로 일단은 나중에 다시.
		//
	}

	void Session::NetUpdate()
	{

		if (m_readBytes)
		{
			memcpy(m_writeBuffer, m_readBuffer, m_readBytes);

			m_writeBytes += m_readBytes;
			m_readBytes = 0;
		}
		if (m_writeBytes)
		{
			int nSent = m_pClient->Send(m_writeBuffer, m_writeBytes);

			// 
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

		int nRead = m_pClient->Recv(m_readBuffer, buflen);
		
		m_readBytes += nRead;
	}

	void Session::SetClient(Client* pClient)
	{
		m_sessionId = ++s_sessionIdCounter;

		m_pClient = pClient;

		m_pClient->SetSessionId(m_sessionId);
	}
}