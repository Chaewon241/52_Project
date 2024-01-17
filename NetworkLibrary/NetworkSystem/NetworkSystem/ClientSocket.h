#pragma once
#include "WinSock.h"

typedef int SessionId;

class ClientSocket : public WinSock
{
public:
    void SetSessionId(SessionId sessionId) { m_sessionId = sessionId; }
    SessionId GetSessionId() { return m_sessionId; }

    ClientSocket() = default;
    virtual ~ClientSocket() = default;

private:
    SessionId m_sessionId = 0;
};

