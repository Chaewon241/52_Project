#pragma once

#include "CircularQueue.h"
#include "MyProtocol.h"

class ClientSocket;
class WinSock;

typedef int SessionId;

class Client
{
public:
	Client() = default;
	~Client() = default;
	
	ClientSocket* GetSocket() { return m_clientSocket; }
	void SetSocket(ClientSocket* socket) { m_clientSocket = socket; }

	void Start();
	void Update();
	void NetUpdate();
	void Stop();

	void OnNetError(int errorCode, const char* errorMsg, WinSock* pSocket);

	void OnReceive();
	void OnSend();
	void OnClose();

	void ClientLoop();

	void SetSessionId(SessionId sessionId) { m_sessionId = sessionId; }
	SessionId GetSessionId() { return m_sessionId; }
private:
	ClientSocket* m_clientSocket;
	SessionId m_sessionId = 0;

	char* m_sendBuffer = nullptr;
	char* m_recvBuffer = nullptr;

	Curtaincall::CircularQueue* m_RecvQueue;
	Curtaincall::CircularQueue* m_SendQueue;
};

