#pragma once

#include "../MyFirstTcpServer/WinSock.h"
#include "WinSockBase.h"

#include <process.h>
#include <Windows.h>


#pragma comment (lib, "Ws2_32.lib")

class CEvent
{
public:
	CEvent(bool bInitialState = false, bool bManualReset = false, TCHAR* pEventName = nullptr)
	{
		m_hEvent = CreateEvent(NULL, bManualReset, bInitialState, pEventName);
	}

	~CEvent()
	{
		if (m_hEvent != INVALID_HANDLE_VALUE)
		{
			CloseHandle(m_hEvent);
		}
	}

	void	Set() { SetEvent(m_hEvent); }
	HANDLE	GetHandle() { return m_hEvent; }

private:
	HANDLE	m_hEvent;
};

class WinSockBase
{

public:
	WinSockBase(void);
	virtual ~WinSockBase(void);
};

// CAsyncSocket�� �����ؼ� Ŭ������ �����ϴ�.
// �ش� Ŭ������ ����ؼ� ������ �����ϴ� �ڵ� �ۼ�
// eventSelect ������ �����ؼ� �����ϴ�.
// eventSelect ������ ������ Ȯ���غ��ϴ�.
//

class WinSockClient : public WinSockBase
{
public:
	WinSockClient() {}
	virtual ~WinSockClient() {}

	bool Connect();

	void DisConnect();

	bool DoUpdate();

	void CloseSocket();

private:
	SOCKET    m_clientSocket = INVALID_SOCKET;

	WSAEVENT m_clientEvent = WSA_INVALID_EVENT;

	bool m_isConnected = false;

	char m_sendBuffer[BUFSIZE] = "I'm MetalDragon";
};

bool WinsockInit();
void WinsockCleanUp();