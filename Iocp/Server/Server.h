#pragma once

#ifndef IOCPSERVER_H
#define IOCPSERVER_H

#include <mswsock.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

// 라이브러리 링크 (Visual Studio 등에서 필요)
#pragma comment(lib, "Ws2_32.lib")

#define DEFAULT_PORT        "5001"
#define MAX_BUFF_SIZE       8192
#define MAX_WORKER_THREAD   128

// client 상태
typedef enum _IO_OPERATION 
{
    ClientIoAccept,
    ClientIoRead,
    ClientIoWrite
} IO_OPERATION, * PIO_OPERATION;

// 소켓에 연결되는 컨텍스트정보
typedef struct _PER_IO_CONTEXT 
{
    WSAOVERLAPPED               Overlapped;
    char                        Buffer[MAX_BUFF_SIZE];
    WSABUF                      wsabuf;
    int                         nTotalBytes;
    int                         nSentBytes;
    IO_OPERATION                IOOperation;
    SOCKET                      SocketAccept;

    struct _PER_IO_CONTEXT* pIOContextForward;
} PER_IO_CONTEXT, * PPER_IO_CONTEXT;

//
// For AcceptEx, the IOCP key is the PER_SOCKET_CONTEXT for the listening socket,
// so we need to another field SocketAccept in PER_IO_CONTEXT. When the outstanding
// AcceptEx completes, this field is our connection socket handle.
// 
// AcceptEx의 경우 IOCP 키는 듣기 소켓의 PER_SOKET_CONTEX이므로
// PER_IO_CONTEXT에서 다른 SocketAccept 필드를 입력해야 합니다.
// 해결되지 않은 AcceptEx가 완료되면 이 필드가 연결 소켓 핸들입니다.
//

// 리스닝 소켓을 위한 IOCP key
typedef struct _PER_SOCKET_CONTEXT 
{
    SOCKET                      Socket;
    
    LPFN_ACCEPTEX               fnAcceptEx;             // AcceptEx 함수의 포인터

    //
    //linked list for all outstanding i/o on the socket
    //
    PPER_IO_CONTEXT             pIOContext;
    struct _PER_SOCKET_CONTEXT* pCtxtBack;
    struct _PER_SOCKET_CONTEXT* pCtxtForward;
} PER_SOCKET_CONTEXT, * PPER_SOCKET_CONTEXT;

// 명령행 인자를 분석하여 프로그램의 옵션을 설정하고, 올바른 옵션이 제공되었는지 확인하는 함수
BOOL ValidOptions(int argc, char* argv[]);

BOOL WINAPI CtrlHandler(
    DWORD dwEvent
);

BOOL CreateListenSocket(void);

unsigned int WINAPI WorkerThread(LPVOID WorkContext);

PPER_SOCKET_CONTEXT UpdateCompletionPort(
    SOCKET s,
    IO_OPERATION ClientIo,
    BOOL bAddToList
);
//
// bAddToList is FALSE for listening socket, and TRUE for connection sockets.
// As we maintain the context for listening socket in a global structure, we
// don't need to add it to the list.
//

VOID CloseClient(
    PPER_SOCKET_CONTEXT lpPerSocketContext,
    BOOL bGraceful
);

PPER_SOCKET_CONTEXT CtxtAllocate(
    SOCKET s,
    IO_OPERATION ClientIO
);

VOID CtxtListFree(
);

VOID CtxtListAddTo(
    PPER_SOCKET_CONTEXT lpPerSocketContext
);

VOID CtxtListDeleteFrom(
    PPER_SOCKET_CONTEXT lpPerSocketContext
);

#endif