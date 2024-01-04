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
		// m_pClient->Send�� �ٷ� ȣ������ �ʰ� m_writeBuffer�� �����صд�.
		//
	}

	void Session::Read(char* pData, int len)
	{
		// ����� ���ذ� ������ �����.
		// ���Ͽ� m_readBuffer�� ������ �о���� �Ѵ�.
		// TODO: ���� ��Ŷ�� ��� �Ľ��� ���ΰ��� ���� ������ �ٲ�Ƿ� �ϴ��� ���߿� �ٽ�.
		//
	}

	void Session::NetUpdate()
	{

		if (m_readBytes)
		{
			// ���� ���ۿ� �����Ͱ� ������ ��ȿ�� ��Ŷ���� Ȯ���ϰ�, ��ȿ�� ��Ŷ�̸� �޽����� ó���Ѵ�.
			// �� Read�� �ϰ� m_readBuffer�� �����͸� �м� -> ��ȿ�� ��Ŷ���� Ȯ��
			// 
			// struct PacketHeader
			// {
			//     short size;
			//     short id;
			// };
			// 
			// 1. ���� ������ ù 2����Ʈ�� �о ����� Ȯ���غ��ϴ�. ���������̶��? ��! ����
			// 2. ������ ������ ���� ��� 30����Ʈ�� ���, �ڿ� ������ ������ ũ�⸦ �����ؼ�
			// (������ ������ ������ ũ���� �����Ͽ�) 30����Ʈ �̻� �ִ��� Ȯ���Ѵ�. �ƴϸ�? ������ ���ؾ��ϴ� ��
			// 3. ������ ������ �����. �׷��� ��Ŷ ���̵� Ȯ���ؼ� ó���Ϸ� ���� ���ǵ� ���̵� �ƴ�. ��! ����. �α�.
			// 4. ��� ������ ������ ���ٸ� ��Ŷ id ���� ó�� �Լ��� ȣ���ϴ°� ���� ������ ����.
			// 
			// 
			// ���� �������� �� ���� �ϴ��� ���� �����͸� �׳� �۽� ���ۿ� �����غ��ϴ�.
			// m_readBuffer�� ���� ������ 
			// 

			memcpy(m_writeBuffer, m_readBuffer, m_readBytes);

			m_writeBytes += m_readBytes;
			m_readBytes = 0;
		}

		// �۽� ���۴� Ŀ�� �޸𸮿� �޸�ī��
		// �۽� ���ۿ� �����Ͱ� ������ Ŭ���̾�Ʈ���� ����(m_pClient->Send)�Ѵ�.
		// 
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
		
		m_readBytes += nRead;
	}

	void Session::SetClient(Client* pClient)
	{
		m_sessionId = ++s_sessionIdCounter;

		m_pClient = pClient;

		m_pClient->SetSessionId(m_sessionId);
	}
}