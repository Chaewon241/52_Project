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

    // sockaddr_in ����ü ����
    sa.sin_family = AF_INET; // IPv4 �ּ� ü��

    // ���� �ּ�
    inet_pton(AF_INET, "172.217.26.238", &(sa.sin_addr));

    sa.sin_port = htons(80); // HTTP ��Ʈ

    // getnameinfo ȣ��
    // �ּ� ����
    // https://learn.microsoft.com/ko-kr/windows/win32/api/ws2tcpip/nf-ws2tcpip-getaddrinfo
    result = getnameinfo((struct sockaddr*)&sa, sizeof(sa),
        host, NI_MAXHOST,
        service, NI_MAXSERV, 0);

    if (result != 0) {
        printf("getnameinfo failed: %d\n", result);
    }
    else {
        // host servic ���� ���
        printf("Host: %s, Service: %s\n", host, service);

    }

    WSACleanup();
    return 0;
}

/*
������ �̸����� �����ϵ��� ����ٴ� ��!
-> ����� �����Ǵ� ����� ����� �� ����.
�ٵ� ������ �������� �ϸ� �����ڰ� �ش� ������ ���Ӱ� ��Ī�� �����Ǹ� �� �� �ִ�.
�׷��� ���� ȣ��Ʈ �̸��� nrt12s51-in-f14.1e100.net
*/