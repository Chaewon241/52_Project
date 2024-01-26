#pragma once
#include <unordered_map>
#include <vector>

class ServerSocket;
class Session;
class ClientNetworkManager;
class WinSock;

typedef int SessionId;

/// <summary>
/// ����
/// Ŭ���̾�Ʈ�� ������ ��� ������ Ŭ���̾�Ʈ�� ������ �����Ѵ�.
/// ���� ����, �κ� ���� ������ �ֱ�.
/// </summary>

// ServerNetworkManager�� �ٲٱ�
class Server
{
public:
	Server() = default;
	~Server() = default;

	void Start();
	void Stop();
	void Update();
	void NetUpdate();

	void OnAccept();
	void OnReceive(std::shared_ptr<WinSock> pSocket);
	void OnSend(std::shared_ptr<WinSock> pSocket);
	void OnClose(std::shared_ptr<ClientNetworkManager> pClient);
	void onNetError(int errorCode, const char* errorMsg, std::shared_ptr<WinSock> pSocket = nullptr);

	bool IsAllReady();

private:
	std::shared_ptr<ServerSocket> m_pListenSocket;

	std::vector<std::shared_ptr<ClientNetworkManager>> m_peerClients;
	std::unordered_map<SessionId, std::shared_ptr<Session>> m_sessions;

	int m_ClientCount = 0;

	bool m_bAllReady = false;
};

