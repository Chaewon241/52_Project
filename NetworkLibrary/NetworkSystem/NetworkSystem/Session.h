#pragma once

typedef int SessionId;

class ClientSocket;

class Session
{
public:
	SessionId GetSessionId() { return m_sessionId; }
	char* GetReadBuffer() { return m_readBuffer; }

	void SetClient(ClientSocket* pClient);
private:
	ClientSocket* m_pClient = nullptr;

	char* m_readBuffer;
	char* m_writeBuffer;

	int m_readBytes = 0;
	int m_writeBytes = 0;

	SessionId m_sessionId = 0;
	SessionId s_sessionIdCounter;
};

