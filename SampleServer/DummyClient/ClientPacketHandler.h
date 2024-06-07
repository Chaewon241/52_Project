#pragma once

enum
{
	S_TEST = 1
};

class ClientPacketHandler
{
public:
	// 패킷 관리
	static void HandlePacket(BYTE* buffer, int32 len);

	static void Handle_S_TEST(BYTE* buffer, int32 len);
};