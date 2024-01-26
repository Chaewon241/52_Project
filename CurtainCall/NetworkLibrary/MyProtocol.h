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

#define MAX_LEN 100

enum EPacketId	: short
{
	PACKETID_START,

	C2S_ACCESS,
	S2C_ACCESS,
	C2S_READY,
	S2C_START,
	C2S_BROADCAST_MSG,
	S2C_BROADCAST_MSG,

	PACKETID_END,
};

struct PacketHeader
{
	short size;
	EPacketId id;
};

struct PacketC2S_Access : PacketHeader
{
	char nickname[MAX_LEN];
};

struct PacketS2C_Access : PacketHeader
{
	std::byte result;
};

struct PacketC2S_BroadcastMsg : PacketHeader
{
	char clientMessage[MAX_LEN];

	/*virtual char* Serialize() override
	{
		char* sendBuffer = new char[size + 1];

		sendBuffer[0] = size / 10 + '0';
		sendBuffer[1] = size % 10 + '0';
		sendBuffer[2] = id / 10 + '0';
		sendBuffer[3] = id % 10 + '0';

		memcpy(sendBuffer + 4, clientMessage, size + 1);

		return sendBuffer;
	}
	virtual void DeSerialize(char buffer[]) {};*/
	
};

struct PacketS2C_BroadcastMsg : PacketHeader
{
	char serverMessage[MAX_LEN];

	//virtual char* Serialize() { return nullptr; };
	//virtual void DeSerialize(char buffer[]) override
	//{
	//	size = static_cast<short>(buffer[0] - '0') * 10 + static_cast<short>(buffer[1] - '0');
	//	id = static_cast<EPacketId>((buffer[2] - '0') * 10 + (buffer[3] - '0'));
	//	//serverMessage = buffer + 4 + '\0';
	//}
};

struct PacketC2S_READY : PacketHeader
{
	short readyState;
};

struct PacketS2C_READY : PacketHeader
{
	short readyState;
};

#pragma pack (pop)