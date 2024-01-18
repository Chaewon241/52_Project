// ��ǻ��, ���α׷����� ����ü ���� ��Ģ�� �ٸ� �� �ִ�.
// ����ü�� �����ϰ� #pragma pack�� ���� ���ϴ´�� ������ �� �ִ�.

// ��Ŷ�� ũ�� �ʰ� ���� ũ��ε� ����� ���� ���Ƽ�
// ��Ŷ ���Ǹ� ����ü�� �ϰ� 1����Ʈ ��ŷ�� �ϴ� ��찡 ����.
// ������ ������ ����.
// ������ ��� ������ Ÿ���� �������� �ʴ�.
// �� �� �����Ӱ� ����ϱ� ���� �ø�������� ��Ŷ ����� ����ų� ���� ���̺귯���� ����Ѵ�.
// ������ ���̺귯���δ� FlatBuffer�� �ִ�.
#pragma once
#include <cstddef>
#include <string>

#pragma pack (push, 1)

enum EPacketId	: short
{
	PACKETID_START,

	C2S_ACCESS,
	S2C_ACCESS,
	C2S_BROADCAST_MSG,
	S2C_BROADCAST_MSG,

	PACKETID_END,
};

struct PacketHeader
{
	short size;
	EPacketId id;

	virtual char* Serialize() = 0;
	virtual void DeSerialize(char* buffer) = 0;
};

struct PacketC2S_Access : PacketHeader
{
	char* nickname;

	virtual char* Serialize() override { return nullptr; }
	virtual void DeSerialize(char* buffer) {};
};

struct PacketS2C_Access : PacketHeader
{
	std::byte result;
	virtual char* Serialize() { return nullptr; };
	virtual void DeSerialize(char* buffer) {};
};

struct PacketC2S_BroadcastMsg : PacketHeader
{
	char* clientMessage;

	virtual char* Serialize() override
	{
		char* sendBuffer = new char[size + 1];

		sendBuffer[0] = size / 10 + '0';
		sendBuffer[1] = size % 10 + '0';
		sendBuffer[2] = id / 10 + '0';
		sendBuffer[3] = id % 10 + '0';

		memcpy(sendBuffer + 4, clientMessage, size + 1);

		return sendBuffer;
	}
	virtual void DeSerialize(char* buffer) {};
	
};

struct PacketS2C_BroadcastMsg : PacketHeader
{
	char* serverMessage;

	virtual char* Serialize() { return nullptr; };
	virtual void DeSerialize(char* buffer) override
	{
		size = static_cast<short>(buffer[0] - '0') * 10 + static_cast<short>(buffer[1] - '0');
		id = static_cast<EPacketId>((buffer[2] - '0') * 10 + (buffer[3] - '0'));;
		serverMessage = buffer + 4 + '\0';
	}
};

#pragma pack (pop)