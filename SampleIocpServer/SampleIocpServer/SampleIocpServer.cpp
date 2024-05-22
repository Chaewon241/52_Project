#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <ws2tcpip.h>
#include <process.h>

#pragma comment(lib, "ws2_32.lib")

#define BUF_SIZE 100
#define READ	3
#define	WRITE	5

// 클라이언트 정보
typedef struct    // socket info
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAdr;
} PER_HANDLE_DATA, * LPPER_HANDLE_DATA;

// 버퍼 정보, read write 정보
typedef struct    // buffer info
{
	OVERLAPPED overlapped;
	WSABUF wsaBuf;
	char buffer[BUF_SIZE];
	int rwMode;    // READ or WRITE
} PER_IO_DATA, * LPPER_IO_DATA;

unsigned int WINAPI EchoThreadMain(LPVOID CompletionPortIO);
void ErrorHandling(std::string message);

int main(int argc, char* argv[])
{
	WSADATA	wsaData;
	HANDLE hComPort;
	SYSTEM_INFO sysInfo;
	LPPER_IO_DATA ioInfo;
	LPPER_HANDLE_DATA handleInfo;

	SOCKET hServSock;
	SOCKADDR_IN servAdr;
	DWORD recvBytes, i, flags = 0;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hComPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
	GetSystemInfo(&sysInfo);

	for (i = 0; i < sysInfo.dwNumberOfProcessors; i++)
		_beginthreadex(NULL, 0, EchoThreadMain, hComPort, 0, NULL);

	hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);
	memset(&servAdr, 0, sizeof(servAdr));
	servAdr.sin_family = AF_INET;
	servAdr.sin_port = htons(7777);
	if (inet_pton(AF_INET, "127.0.0.1", &servAdr.sin_addr) <= 0) {
		perror("inet_pton() failed");
		exit(EXIT_FAILURE);
	}

	bind(hServSock, (SOCKADDR*)&servAdr, sizeof(servAdr));
	listen(hServSock, 5);

	while (1)
	{
		SOCKET hClntSock;
		SOCKADDR_IN clntAdr;
		int addrLen = sizeof(clntAdr);

		hClntSock = WSAAccept(hServSock, NULL, NULL, NULL, 0);
		if (hClntSock == SOCKET_ERROR)
			continue;

		// 소켓 정보 저장하기
		handleInfo = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));
		if (handleInfo == NULL)
			ErrorHandling("malloc() error");

		handleInfo->hClntSock = hClntSock;
		memcpy(&(handleInfo->clntAdr), &clntAdr, addrLen);

		CreateIoCompletionPort((HANDLE)hClntSock, hComPort, (DWORD_PTR)handleInfo, 0);

		// 버퍼 정보 저장하기
		ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		if (ioInfo == NULL)
			ErrorHandling("malloc() error");
		memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
		ioInfo->wsaBuf.len = BUF_SIZE;
		ioInfo->wsaBuf.buf = ioInfo->buffer;
		ioInfo->rwMode = READ;

		// 에코 서버기 때문에 Recv 걸어둔다!
		if (WSARecv(handleInfo->hClntSock, &(ioInfo->wsaBuf), 1, 
			&recvBytes, &flags, &(ioInfo->overlapped), NULL) == SOCKET_ERROR)
		{
			if (WSAGetLastError() != WSA_IO_PENDING)
			{
				closesocket(handleInfo->hClntSock);
				free(handleInfo);
				free(ioInfo);
				continue;
			}
		}
	}
	return 0;
}

unsigned int WINAPI EchoThreadMain(LPVOID pComPort)
{
	HANDLE hComPort = (HANDLE)pComPort;
	SOCKET sock;
	DWORD bytesTrans;
	LPPER_HANDLE_DATA handleInfo;
	LPPER_IO_DATA ioInfo;
	DWORD flags = 0;

	while (1)
	{
		// 소켓 정보와 overlapped 정보(버퍼 정보)를 추출
		// IO가 완료가 되면 소켓과 cp오브젝트가 연결되었으면, 소켓에 일어나는 IO 작업들은 여기에 다 옴.
		BOOL result = GetQueuedCompletionStatus(hComPort, &bytesTrans, 
			(PULONG_PTR)&handleInfo, (LPOVERLAPPED*)&ioInfo, INFINITE);
		if (result == 0)
		{
			closesocket(handleInfo->hClntSock);
			free(handleInfo);
			free(ioInfo);
			continue;
		}

		sock = handleInfo->hClntSock;

		if (ioInfo->rwMode == READ)
		{
			puts("message received!");
			if (bytesTrans == 0)    // EOF 전송 시
			{
				closesocket(sock);
				free(handleInfo); free(ioInfo);
				continue;
			}
			ioInfo->buffer[bytesTrans] = '\0';
			printf("Message from client : %s", ioInfo->buffer);
			
			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = bytesTrans;
			ioInfo->rwMode = WRITE;
			WSASend(sock, &(ioInfo->wsaBuf),
				1, NULL, 0, &(ioInfo->overlapped), NULL);

			ioInfo = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
			memset(&(ioInfo->overlapped), 0, sizeof(OVERLAPPED));
			ioInfo->wsaBuf.len = BUF_SIZE;
			ioInfo->wsaBuf.buf = ioInfo->buffer;
			ioInfo->rwMode = READ;
			WSARecv(sock, &(ioInfo->wsaBuf),
				1, NULL, &flags, &(ioInfo->overlapped), NULL);
		}
		else
		{
			ioInfo->buffer[bytesTrans] = '\0';
			printf("Message to client : %s", ioInfo->buffer);
			puts("sent!");
			free(ioInfo);
		}
	}
	return 0;
}
void ErrorHandling(std::string msg)
{
	std::cout << msg << std::endl;
	exit(1);
}
