#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
    WSADATA wsaData;
    SOCKET listeningSocket, clientSocket;
    struct sockaddr_in server, client;

    const char* message = "Hello from server";

    // 1. 윈속을 초기화한다.
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        printf("Failed to initialize Winsock. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    // 2. 소켓을 생성한다.
    // listeningSocket 연결을 받아주는용
    if ((listeningSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;    // tcp udp 인터넷만 쓰것다.
    server.sin_addr.s_addr = INADDR_ANY;    // 특정 포트에서 기다릴 것이다
    server.sin_port = htons(7777);          // 포트

    // 3. 소켓을 bind한다.
    // 클라가 아니니까 주소 바인딩
    if (bind(listeningSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed. Error Code: %d\n", WSAGetLastError());
        return 1;
    }

    // 4. 소켓을 listen 상태로 만든다.
    // 소켓 상태가 접속이 들어오면 뭔가를 받아옴.
    listen(listeningSocket, 3);     // 해당 함수가 리슨하는게 아니라 소켓 상태를 리슨 상태로 만듦


    fd_set readfds;
    int clientLen = sizeof(struct sockaddr_in);

    while (1)
    {
        printf("Waiting for connections...\n");

        FD_ZERO(&readfds);

        FD_SET(listeningSocket, &readfds);

        // 6. 소켓에 연결된 클라이언트가 있는지 확인한다.
        // 블로킹함수 -> 누가 들어오기 전까지는 넘어가지 않음 마치 scanf. 기다리고 있음.

        if (select(listeningSocket + 1, &readfds, NULL, NULL, NULL) == SOCKET_ERROR) {
            printf("select function failed. Error Code: %d\n", WSAGetLastError());
            break;
        }


        if (FD_ISSET(listeningSocket, &readfds)) {
            if ((clientSocket = accept(listeningSocket, (struct sockaddr*)&client, &clientLen)) == INVALID_SOCKET) {
                printf("accept failed. Error Code: %d\n", WSAGetLastError());
                break;
            }

            printf("Connection accepted\n");

            // 접속한 클라이언트에 메시지를 송신하고 소켓을 닫는다.
            send(clientSocket, message, strlen(message), 0);
            closesocket(clientSocket);
        }
    }

    // 7. listen 소켓을 닫는다.
    closesocket(listeningSocket);

    // 8. 윈속을 종료한다.
    WSACleanup();

    return 0;
}
/*
시스템 프로그래밍의 일부 네트워크 프로그래밍
    소켓 프로그래밍
        윈속 프로그래밍
        리눅스 소켓 프로그래밍

소켓 : 파일을 열고 닫을 때

C언어에서 파일을 열 때: FILE*
윈도우즈: 핸들

시스템 자원 관리는 커널(os)이 하고
파일, 소켓, 메모리, ..
파일처럼 메모리 영역이 있고(송수신 버퍼) NIC를 통해 패킷을 보내고 받아서 처리하는 버퍼를 관리
이 모든 걸 소켓 핸들을 가지고 -> 소켓은 포트와 1:1

윈도우즈 소켓 프로그래밍
소켓 모델
1. 블로킹 / 넌블로킹
2. 이벤트 셀렉트 : 네트워크 이벤트 감지 방법
3. 오버랩드 : 송수신 데이터 효율 ( 이벤트, 콜백, IOCP)
4. IOCP : 네트워크 작업의 완료를 파악하는 방법
*/