#pragma once
#include "NetworkSystem.h"

#include <unordered_map>
#include <vector>

typedef int SessionId;

class ServerSocket;
class Session;
class ClientSocket;
class WinSock;

class Server
{
public:
	Server() = default;
	~Server() = default;

	void Start();
	void Stop();
	void Update();

	void OnAccept();
	void OnReceive(WinSock* pSocket);
	void OnSend(WinSock* pSocket);
	void OnClose(WinSock* pSocket);
	void onNetError(int errorCode, const char* errorMsg, WinSock* pSocket = nullptr);

	void ServerLoop();

private:
	NetworkSystem m_networkSystem;
	ServerSocket* m_pServerSocket;

	std::vector<ClientSocket*> m_clients;
	std::unordered_map<SessionId, Session*> m_sessions;

	int m_ClientCount = 0;
};

