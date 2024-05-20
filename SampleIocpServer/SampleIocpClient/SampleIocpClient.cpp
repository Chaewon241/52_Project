#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#include <iostream>

void ErrorHandling(const char* message);

#pragma comment(lib, "ws2_32.lib")

int main()
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	// 소켓 생성
	SOCKET hSocket = WSASocket(PF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	SOCKADDR_IN recvAddr;
	memset(&recvAddr, 0, sizeof(recvAddr));
	recvAddr.sin_family = AF_INET;
	::inet_pton(AF_INET, "127.0.0.1", &recvAddr.sin_addr);
	recvAddr.sin_port = ::htons(2738);

	if (connect(hSocket, (SOCKADDR*)&recvAddr, sizeof(recvAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");

	WSAEVENT event = WSACreateEvent();

	WSAOVERLAPPED overlapped;
	memset(&overlapped, 0, sizeof(overlapped));

	overlapped.hEvent  = event;

	WSABUF dataBuf;
	char message[1024] = { 0, };
	DWORD sendBytes = 0;
	int recvBytes = 0;
	DWORD flags = 0;

	while (true)
	{
		flags = 0;
		printf("전송할데이터(종료를원할시exit)\n");

		std::cin >> message;

		if (!strcmp(message, "exit")) break;

		dataBuf.len = strlen(message);
		dataBuf.buf = message;
		
		std::cout << "client send : " << message << std::endl;

		if (WSASend(hSocket, &dataBuf, 1, (LPDWORD)&sendBytes, 0, &overlapped, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
				ErrorHandling("WSASend() error");
		}

		std::cout << "전송된바이트수: " << sendBytes << std::endl;

		if (WSARecv(hSocket, &dataBuf, 1, (LPDWORD)&recvBytes, (LPDWORD)&flags, &overlapped, NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() == WSA_IO_PENDING)
			{
				::WSAWaitForMultipleEvents(1, &event, TRUE, WSA_INFINITE, FALSE);
				::WSAGetOverlappedResult(hSocket, &overlapped, (LPDWORD)&recvBytes, FALSE, &flags);
			}
			else
			{
				ErrorHandling("WSASend() error");
				break;
			}
		}
		dataBuf.buf[recvBytes] = '\0';
		printf("Recv[%s]\n", dataBuf.buf);
	}

	closesocket(hSocket);

	WSACleanup();

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);

	exit(1);
}