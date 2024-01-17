#pragma once
#include "NetworkSystem.h"

class ClientSocket;
class Client
{
public:
	Client() = default;
	~Client() = default;

	void Start();
	void Update();
	void Stop();

	void OnReceive();
	void OnSend();

	void ClientLoop();
private:
	ClientSocket* m_clientSocket;
	NetworkSystem m_networkSystem;
};

