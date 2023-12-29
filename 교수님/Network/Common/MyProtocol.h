#pragma once


// 패킷이 크지 않고 고정 크기로도 충분할 때가 많아서
// 패킷 정의를 구조체로 하고 1바이트 패킹을 하는 경우가 많습니다.
// 장점 : 구현이 쉽다.
// 단점 : 멤버 데이터 타입이 자유롭지 않다.
// 좀 더 자유롭게 사용하기 위해 시리얼라이즈 패킷 모듈을 만들거나 기존 라이브러리를 사용하기도 한다.
// ex)  FlatBuffer

// 구조체 정렬은 CPU에 친화적인데, 이건 CPU에 친화적일 필요가 없다.
// 친화적 이라는 뜻은 캐시 메모리에 올라가는 데에 더 효율적이라는 뜻.

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
	// 필요한 데이터를 추가하게 됩니다.
	char helloKey[100];
};

struct PacketC2S_Welcome : PacketHeader
{
	// 필요한 데이터를 추가하게 됩니다.
	char welcomeKey[100];
};

#pragma pack (pop)
