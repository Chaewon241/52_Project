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
		// 송신 버퍼는 커널 메모리에 메모리카피
		// 송신 버퍼에 데이터가 있으면 클라이언트에게 전송(m_pClient->Send)한다.
		// 수신 버퍼에 데이터가 있으면 유효한 패킷인지 확인하고, 유효한 패킷이면 메시지를 처리한다.
		// ↑ Read를 하고 m_readBuffer의 데이터를 분석 -> 유효한 패킷인지 확인
		// 
	}

	void Session::SetClient(Client* pClient)
	{
		m_sessionId = ++s_sessionIdCounter;

		m_pClient = pClient;

		m_pClient->SetSessionId(m_sessionId);
	}
}