#pragma once


// ��Ŷ�� ũ�� �ʰ� ���� ũ��ε� ����� ���� ���Ƽ�
// ��Ŷ ���Ǹ� ����ü�� �ϰ� 1����Ʈ ��ŷ�� �ϴ� ��찡 �����ϴ�.
// ���� : ������ ����.
// ���� : ��� ������ Ÿ���� �������� �ʴ�.
// �� �� �����Ӱ� ����ϱ� ���� �ø�������� ��Ŷ ����� ����ų� ���� ���̺귯���� ����ϱ⵵ �Ѵ�.
// ex)  FlatBuffer

// ����ü ������ CPU�� ģȭ���ε�, �̰� CPU�� ģȭ���� �ʿ䰡 ����.
// ģȭ�� �̶�� ���� ĳ�� �޸𸮿� �ö󰡴� ���� �� ȿ�����̶�� ��.

#pragma pack (push, 1)

enum EPacketId
{
	C2S_HELLO = 1,
	S2C_HELLO = 2,
};

struct PacketHeader
{
	short size;
	short id;
};

struct PacketC2S_Hello : PacketHeader
{
	// �ʿ��� �����͸� �߰��ϰ� �˴ϴ�.
	char helloKey[100];
};

struct PacketC2S_Welcome : PacketHeader
{
	// �ʿ��� �����͸� �߰��ϰ� �˴ϴ�.
	char welcomeKey[100];
};

#pragma pack (pop)
