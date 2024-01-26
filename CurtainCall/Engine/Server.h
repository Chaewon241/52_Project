#pragma once
#include <unordered_map>
#include <vector>

class ServerSocket;
class Session;
class ClientNetworkManager;
class WinSock;

typedef int SessionId;

/// <summary>
/// 서버
/// 클라이언트의 정보를 들고 있으며 클라이언트와 연결을 진행한다.
/// 게임 상태, 로비 상태 가지고 있기.
/// </summary>

// ServerNetworkManager로 바꾸기
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

