#include "WinSockBase.h"

bool WinsockInit()
{
    return false;
}

void WinsockCleanUp()
{
}

WinSockBase::WinSockBase(void)
{
}

WinSockBase::~WinSockBase(void)
{
}

bool WinSockBase::Start(bool bBeginAndRun)
{
    if (bBeginAndRun)	m_RunEvent.Set();

    return true;
}

bool WinSockBase::Stop(DWORD dwWaitTime)
{
    return false;
}
