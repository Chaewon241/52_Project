#pragma once

typedef int SessionId;

constexpr int SND_BUF_SIZE = 1024;
constexpr int RCV_BUF_SIZE = 9604;

class ClientSocket;

class Session
{
public:
	Session();
	~Session();

	void Write(char* pData, int len);

	PacketC2S_BroadcastMsg* Read(char* pData, int len);

	void NetUpdate();

	void ReadUpdate();

	void SetClient(ClientSocket* pClient);

	SessionId GetSessionId() { return m_sessionId; }

	char* GetReadBuffer() { return m_readBuffer; }

private:
	ClientSocket* m_pClient = nullptr;

	static constexpr int BUF_SIZE = 1024;

	char* m_readBuffer;
	char* m_writeBuffer;

	int m_readBytes = 0;
	int m_writeBytes = 0;

	SessionId m_sessionId = 0;
	SessionId s_sessionIdCounter = 0;
};

