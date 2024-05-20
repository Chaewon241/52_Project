#pragma once
#include <mswsock.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <iostream>

// 라이브러리 링크 (Visual Studio 등에서 필요)
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT        "7777"
#define MAX_BUFF_SIZE       1024
#define MAX_WORKER_THREAD   128

typedef enum _IO_OPERATION {
    ClientIoAccept,
    ClientIoRead,
    ClientIoWrite
} IO_OPERATION, * PIO_OPERATION;

typedef struct _PER_IO_CONTEXT {
    WSAOVERLAPPED               Overlapped;
    char                        Buffer[MAX_BUFF_SIZE];
    WSABUF                      wsabuf;
    int                         nTotalBytes;
    int                         nSentBytes;
    IO_OPERATION                IOOperation;
    SOCKET                      SocketAccept;

    struct _PER_IO_CONTEXT* pIOContextForward;
} PER_IO_CONTEXT, * PPER_IO_CONTEXT;

