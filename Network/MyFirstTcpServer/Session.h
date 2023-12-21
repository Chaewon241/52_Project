#pragma once

#pragma once
#include "Types.h"
#include <string>

namespace netfish
{
	class Client;

	class Session
	{
	public:
		Session();

		~Session();

		void Write(const char* pData, int len);

		void Read(char* pData, int len);

		void NetUpdate();

		void SetClient(Client* pClient);

		SessionId GetSessionId() { return m_sessionId; }

		char* GetReadBuffer() { return m_readBuffer; }

	private:

		Client* m_pClient = nullptr;

		static constexpr int BUF_SIZE = 1024;

		char m_readBuffer[BUF_SIZE] = {};
		char m_writeBuffer[BUF_SIZE] = {};

		SessionId m_sessionId = 0;

		static SessionId s_sessionIdCounter;
	};
}
