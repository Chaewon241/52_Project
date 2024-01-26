#pragma once
#include "../NetworkLibrary/MyProtocol.h"

#include <queue>

class ClientSocket;
class ClientNetworkManager;

typedef int SessionId;

class Session
{
public:
	Session();
	~Session();

	void Write(char* pData, int len);

	void Read(char* pData, int len);

	void NetUpdate();

	void ReadUpdate();

	void SetClient(std::shared_ptr<ClientNetworkManager> pClient);

	bool GetReadyState() { return m_bReady; }

	SessionId GetSessionId() { return m_sessionId; }

	void PushSendQueue(char* c, int len) { m_sendQueue.push({ c, len }); }
	void PushRecvQueue(char* c, int len) { m_recvQueue.push({ c, len }); }

	void PopSendQueue() { m_sendQueue.pop(); }
	void PopRecvQueue() { m_recvQueue.pop(); }

	bool IsSendQueueEmpty() { return m_sendQueue.empty(); }
	bool IsRecvQueueEmpty() { return m_recvQueue.empty(); }

	char* GetRecvQueueFrontBuffer() { return m_recvQueue.front().first; }
	char* GetSendQueueFrontBuffer() { return m_sendQueue.front().first; }

	int GetRecvQueueFrontLen() { return m_recvQueue.front().second; }
	int GetSendQueueFrontLen() { return m_sendQueue.front().second; }

	void SetHostSession() { m_bIsHost = true; }

	void Finalize();

private:
	std::queue<std::pair<char*, int>> m_sendQueue;
	std::queue<std::pair<char*, int>> m_recvQueue;

	int m_readBytes = 0;
	int m_writeBytes = 0;

	std::shared_ptr<ClientNetworkManager> m_pPeerClient = nullptr;
	SessionId m_sessionId = 0;
	SessionId s_sessionIdCounter = 0;

	bool m_bIsHost = false;

	bool m_bReady = false;

	std::string m_NickName = {};
};

