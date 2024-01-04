#pragma once

namespace netfish
{
	typedef int SessionId;

	enum class NetMessageType
	{
		Connect,
		Disconnect,
		Data,
	};

	struct NetMessage
	{
		NetMessageType type;
	};

	struct NetMsg
	{
		SessionId id;
		char* pBuf; 
	};
}