#pragma once

// InputSystem 이나 RenderSystem 처럼 네트워크에 관한 책임을 지는 걸 하나 만든다.
// 싱글턴으로 만들어도 됩니다.
class NetworkSystem
{
	// 초기화 : 통신을 할수 있는 준비
	// 서버에 연결
	// 연결 된 상태인지 관리

	Init();

	DoUdate()
	{
		WinSockClient 의 DoUpdate 가 호출된다.
	}

	보내는 쪽에서 구조체를 만들어서 파라메터로 하고. void*
	bool PostMsg(void* pMsg, const int size);


	PopMsg();

	WinSockClient m_Client;

	std::queue<NetMsg> m_netMsgQ;
};

