#pragma once

#include <process.h>
#include <Windows.h>

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

	virtual	void	DoUpdate() = 0;

	bool	Start(bool bBeginAndRun = true);
	bool	Stop(DWORD dwWaitTime = INFINITE); // millisec

private:
	CEvent	m_RunEvent;
	CEvent	m_QuitEvent;

	WinSockBase(const WinSockBase& rhs) = delete;
	WinSockBase& operator=(const WinSockBase& rhs) = delete;
};

bool WinsockInit();
void WinsockCleanUp();