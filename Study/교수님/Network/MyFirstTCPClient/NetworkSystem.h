#pragma once

// InputSystem �̳� RenderSystem ó�� ��Ʈ��ũ�� ���� å���� ���� �� �ϳ� �����.
// �̱������� ���� �˴ϴ�.
class NetworkSystem
{
	// �ʱ�ȭ : ����� �Ҽ� �ִ� �غ�
	// ������ ����
	// ���� �� �������� ����

	Init();

	DoUdate()
	{
		WinSockClient �� DoUpdate �� ȣ��ȴ�.
	}

	������ �ʿ��� ����ü�� ���� �Ķ���ͷ� �ϰ�. void*
	bool PostMsg(void* pMsg, const int size);


	PopMsg();

	WinSockClient m_Client;

	std::queue<NetMsg> m_netMsgQ;
};

