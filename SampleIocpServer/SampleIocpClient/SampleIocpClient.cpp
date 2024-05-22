#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 1024
void ErrorHandling(std::string message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;

	char message[BUF_SIZE];
	int strLen, readLen;

	if (argc != 3) {
		printf("Usage: %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	SOCKADDR_IN serverAddr;
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);
	serverAddr.sin_port = ::htons(7777);

	if (connect(hSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");
	else
		puts("Connected...........");

	while (1)
	{
		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);
		if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;

		strLen = strlen(message);
		send(hSocket, message, strLen, 0);

		readLen = 0;
		while (1)
		{
			// TCP의 특성상 데이터에 경계가 없음. 그래서 보낸거 다 받고 Read를 해야한다.
			readLen += recv(hSocket, &message[readLen], BUF_SIZE - 1, 0);
			if (readLen >= strLen)
				break;
		}
		message[strLen] = 0;
		printf("Message from server: %s", message);
	}

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(std::string msg)
{
	std::cout << msg << std::endl;
	exit(1);
}
