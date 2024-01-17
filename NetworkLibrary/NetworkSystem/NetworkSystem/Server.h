#pragma once
#include "NetworkSystem.h"

#include <unordered_map>
#include <vector>

typedef int SessionId;

class ServerSocket;
class Session;
class ClientSocket;

class Server
{
public:
	Server() = default;
	~Server() = default;

	void Start();
	void Stop();

	void Update();
	void OnReceive();
	void OnSend();

	void ServerLoop();

private:
	NetworkSystem m_networkSystem;

	ServerSocket* m_serverSocket;

	std::vector<ClientSocket*> m_clients;
	std::unordered_map<SessionId, Session*> m_sessions;

	int m_ClientCount = 0;
};

