#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <process.h>

#define BUFSIZE 1024

typedef struct
{
	SOCKET hClntSock;
	SOCKADDR_IN clntAddr;
} PER_HANDLE_DATA, *LPPER_HANDLE_DATA;

typedef struct
{
	OVERLAPPED overlapped;
	char buffer[BUFSIZE];
	WSABUF wsaBuf;
} PER_IO_DATA, *LPPER_IO_DATA;

unsigned int __stdcall CompletionThread(LPVOID pComPort);
void ErrorHandling(const char* message);

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char** argv)
{
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	HANDLE hCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);

	SYSTEM_INFO SystemInfo;
	GetSystemInfo(&SystemInfo);

	for (int i = 0; i < SystemInfo.dwNumberOfProcessors; ++i)
		_beginthreadex(NULL, 0, CompletionThread, (LPVOID)hCompletionPort, 0, NULL);

	SOCKET hServSock = WSASocket(AF_INET, SOCK_STREAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	SOCKADDR_IN servAddr;
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	//servAddr.sin_port = htons(atoi("2738"));
	servAddr.sin_port = htons(2738);

	bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr));
	listen(hServSock, 5);

	LPPER_IO_DATA PerIoData;
	LPPER_HANDLE_DATA PerHandleData;

	int RecvBytes;
	int i, Flags;

	while (TRUE)
	{
		SOCKADDR_IN clntAddr;
		int addrLen = sizeof(clntAddr);

		SOCKET hClntSock = WSAAccept(hServSock, NULL, NULL, NULL, 0);

		PerHandleData = (LPPER_HANDLE_DATA)malloc(sizeof(PER_HANDLE_DATA));
		PerHandleData->hClntSock = hClntSock;
		memcpy(&(PerHandleData->clntAddr), &clntAddr, addrLen);

		CreateIoCompletionPort((HANDLE)PerHandleData->hClntSock, hCompletionPort, (ULONG_PTR)PerHandleData, 0);

		PerIoData = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(&(PerIoData->overlapped), 0, sizeof(OVERLAPPED));
		PerIoData->wsaBuf.len = BUFSIZE;
		PerIoData->wsaBuf.buf = PerIoData->buffer;
		Flags = 0;

		WSARecv(PerHandleData->hClntSock, &(PerIoData->wsaBuf), 1, (LPDWORD)&RecvBytes, (LPDWORD)&Flags, &(PerIoData->overlapped), NULL);
	}

	return 0;
}

unsigned int __stdcall CompletionThread(LPVOID pComPort)
{
	HANDLE hCompletionPort = (HANDLE)pComPort;
	DWORD BytesTransferred;
	LPPER_HANDLE_DATA PerHandleData;
	LPPER_IO_DATA PerIoData;
	DWORD flags;
	BOOL result;

	while (1) 
	{
		// 서버가 걸어놓은 비동기가 완료되었을 때 반환 되는 함수
		GetQueuedCompletionStatus(
			hCompletionPort,
			&BytesTransferred,
			(PULONG_PTR)&PerHandleData, // PerHandleData를 저장할 포인터
			reinterpret_cast<LPOVERLAPPED*>(&PerIoData),
			INFINITE);

		if (BytesTransferred == 0)
		{
			closesocket(PerHandleData->hClntSock);
			free(PerHandleData);
			free(PerIoData);
			continue;
		}

		DWORD bytesSent = 0;
		WSABUF sendBuf = PerIoData->wsaBuf; // 기존의 버퍼를 사용하여 보냄
		result = WSASend(PerHandleData->hClntSock, &sendBuf, 1, &bytesSent, 0, NULL, NULL);
		if (result == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
		{
			printf("WSASend failed with error: %d\n", WSAGetLastError());
			closesocket(PerHandleData->hClntSock);
			free(PerHandleData);
			free(PerIoData);
			continue;
		}

		// 새 I/O 작업을 위해 새로운 PER_IO_DATA 할당
		LPPER_IO_DATA newPerIoData = (LPPER_IO_DATA)malloc(sizeof(PER_IO_DATA));
		memset(&(newPerIoData->overlapped), 0, sizeof(OVERLAPPED));
		newPerIoData->wsaBuf.len = BUFSIZE;
		newPerIoData->wsaBuf.buf = newPerIoData->buffer;

		flags = 0;

		// 새로운 비동기 수신 작업을 시작
		result = WSARecv(PerHandleData->hClntSock, &(newPerIoData->wsaBuf), 1, NULL, &flags, &(newPerIoData->overlapped), NULL);
		if (result == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
		{
			printf("WSARecv failed with error: %d\n", WSAGetLastError());
			closesocket(PerHandleData->hClntSock);
			free(PerHandleData);
			free(newPerIoData);
			continue;
		}
		PerIoData->wsaBuf.buf[BytesTransferred] = '\0';
		printf("Recv[%s]\n", PerIoData->wsaBuf.buf);

		// 기존의 PER_IO_DATA 구조체는 여기서 해제
		free(PerIoData);
	}

	return 0;
}

void ErrorHandling(const char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
