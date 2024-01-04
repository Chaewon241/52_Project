#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

int main()
{
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return 1;
    }

    struct sockaddr_in sa;
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];

    // sockaddr_in 구조체 설정
    sa.sin_family = AF_INET; // IPv4 주소 체계

    // 구글 주소
    inet_pton(AF_INET, "172.217.26.238", &(sa.sin_addr));

    sa.sin_port = htons(80); // HTTP 포트

    // getnameinfo 호출
    // 주소 정보
    // https://learn.microsoft.com/ko-kr/windows/win32/api/ws2tcpip/nf-ws2tcpip-getaddrinfo
    result = getnameinfo((struct sockaddr*)&sa, sizeof(sa),
        host, NI_MAXHOST,
        service, NI_MAXSERV, 0);

    if (result != 0) {
        printf("getnameinfo failed: %d\n", result);
    }
    else {
        // host servic 정보 출력
        printf("Host: %s, Service: %s\n", host, service);

    }

    WSACleanup();
    return 0;
}

/*
도메인 이름으로 접속하도록 만든다는 것!
-> 장비의 아이피는 사용이 변경될 수 있음.
근데 도메인 네임으로 하면 관리자가 해당 도메인 네임과 매칭된 아이피를 줄 수 있다.
그래서 구글 호스트 이름이 nrt12s51-in-f14.1e100.net
*/