#pragma once
#include "Protocol.pb.h"

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

// TODO : 자동화
enum : uint16
{
	PKT_S_TEST = 1,
	PKT_S_LOGIN = 2,
};

// TODO : 자동화
// Custom Handlers
// 아직 등록되지 않은 패킷 아이디로 올 때 처리해주는 함수
bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_S_TEST(PacketSessionRef& session, Protocol::S_TEST& pkt);

/// <summary>
/// 가변데이터를 패킷에 넣을 때 사용하는 핸들러
/// </summary>

class ServerPacketHandler
{
public:
	// TODO : 자동화
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;

		GPacketHandler[PKT_S_TEST] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::S_TEST>(Handle_S_TEST, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	
	// TODO : 자동화
	static SendBufferRef MakeSendBuffer(Protocol::S_TEST& pkt) { return MakeSendBuffer(pkt, PKT_S_TEST); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		// 버퍼 열어주고
		SendBufferRef sendBuffer = GSendBufferManager->Open(packetSize);
		// 헤더 채워주고
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		// 데이터 밀어넣고
		ASSERT_CRASH(pkt.SerializeToArray(&header[1], dataSize));
		// 버퍼 닫아주기
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};

