#pragma once
#define MAX_LEN 100
#include "../NetworkLibrary/MyProtocol.h"

#include <memory>

class ClientSocket;
class WinSock;

typedef int SessionId;

class ClientNetworkManager
{
public:
	ClientNetworkManager() = default;
	~ClientNetworkManager() = default;
	
	std::shared_ptr<ClientSocket> GetSocket() { return m_clientSocket; }
	void SetSocket(std::shared_ptr<ClientSocket> socket) { m_clientSocket = socket; }

	void Start();
	void Update();
	void NetUpdate();
	void Stop();

	void Read(char buf[], int size);
	void Write(char buf[], int size);

	void OnNetError(int errorCode, const char* errorMsg, std::shared_ptr<WinSock> pSocket);

	void OnReceive();
	void OnSend();
	void OnClose();

	void SetSessionId(SessionId sessionId) { m_sessionId = sessionId; }
	SessionId GetSessionId() { return m_sessionId; }

	void SetReadyStateTrue() { m_bReadyState = true; }
	bool GetReadyState() { return m_bReadyState; }

private:
	std::shared_ptr<ClientSocket> m_clientSocket;
	SessionId m_sessionId = 0;

	char m_sendBuffer[MAX_LEN];
	char m_recvBuffer[MAX_LEN];

	bool m_bReadyState = false;
};

