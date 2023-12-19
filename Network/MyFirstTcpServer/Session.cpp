#include "Session.h"
#include "Client.h"

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
		// �۽� ���۴� Ŀ�� �޸𸮿� �޸�ī��
		// �۽� ���ۿ� �����Ͱ� ������ Ŭ���̾�Ʈ���� ����(m_pClient->Send)�Ѵ�.
		// ���� ���ۿ� �����Ͱ� ������ ��ȿ�� ��Ŷ���� Ȯ���ϰ�, ��ȿ�� ��Ŷ�̸� �޽����� ó���Ѵ�.
		// �� Read�� �ϰ� m_readBuffer�� �����͸� �м� -> ��ȿ�� ��Ŷ���� Ȯ��
		// 
	}

	void Session::SetClient(Client* pClient)
	{
		m_sessionId = ++s_sessionIdCounter;

		m_pClient = pClient;

		m_pClient->SetSessionId(m_sessionId);
	}
}