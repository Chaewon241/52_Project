#pragma warning (disable:4127)
#ifdef _IA64_
#pragma warning(disable:4706 4267)
#endif 

#include "pch.h"

#include "Service.h"

#define MAXTHREADS 128

#define xmalloc(s) HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,(s))
#define xfree(p)   {HeapFree(GetProcessHeap(),0,(p)); p = NULL;}

typedef struct _OPTIONS 
{
	char szHostname[64];
	const char* port;
	int nTotalThreads;
	int nBufSize;
	BOOL bVerbose;
} OPTIONS;

typedef struct THREADINFO 
{
	HANDLE hThread[MAXTHREADS];
	SOCKET sd[MAXTHREADS];
} THREADINFO;

static OPTIONS default_options = { "localhost", "5001", 1, 4096, FALSE };
static OPTIONS g_Options;
static THREADINFO g_ThreadInfo;
static BOOL g_bEndClient = FALSE;
static WSAEVENT g_hCleanupEvent[1];

static BOOL WINAPI CtrlHandler(DWORD dwEvent);
static BOOL ValidOptions(char* argv[], int argc);
static VOID Usage(char* szProgramname, OPTIONS* pOptions);
static DWORD WINAPI EchoThread(LPVOID lpParameter);
static BOOL CreateConnectedSocket(int nThreadNum);
static BOOL SendBuffer(int nThreadNum, char* outbuf);
static BOOL RecvBuffer(int nThreadNum, char* inbuf);
static int myprintf(const char* lpFormat, ...);

int main(int argc, char* argv[]) 
{
	WSADATA wsaData;
	ASSERT_CRASH(::WSAStartup(MAKEWORD(2, 2), OUT & wsaData) == 0);

	/* 런타임에 주소 얻어오는 API */
	SOCKET dummySocket = CreateSocket();
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_CONNECTEX, reinterpret_cast<LPVOID*>(&ConnectEx)));
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_DISCONNECTEX, reinterpret_cast<LPVOID*>(&DisconnectEx)));
	ASSERT_CRASH(BindWindowsFunction(dummySocket, WSAID_ACCEPTEX, reinterpret_cast<LPVOID*>(&AcceptEx)));
	Close(dummySocket);


	shared_ptr<ClientService> service = make_shared<ClientService>(NetAddress(L"127.0.0.1", 7777));

	if (!service->Start())
	{
		cout << "connect 이상" << endl;
		return 0;
	}
	
	// 여기부터
	WSADATA WSAData;
	DWORD dwThreadId = 0;
	DWORD dwRet = 0;
	BOOL bInitError = FALSE;
	int nThreadNum[MAXTHREADS];
	int i = 0;
	int nRet = 0;

	for (i = 0; i < MAXTHREADS; i++) 
	{
		g_ThreadInfo.sd[i] = INVALID_SOCKET;
		g_ThreadInfo.hThread[i] = INVALID_HANDLE_VALUE;
		nThreadNum[i] = 0;
	}

	g_hCleanupEvent[0] = WSA_INVALID_EVENT;

	if (!ValidOptions(argv, argc))
		return(1);

	if ((nRet = WSAStartup(MAKEWORD(2, 2), &WSAData)) != 0) {
		myprintf("WSAStartup() failed: %d", nRet);
		return(1);
	}

	if (WSA_INVALID_EVENT == (g_hCleanupEvent[0] = WSACreateEvent()))
	{
		myprintf("WSACreateEvent() failed: %d\n", WSAGetLastError());
		WSACleanup();
		return(1);
	}

	if (!SetConsoleCtrlHandler(CtrlHandler, TRUE)) {
		myprintf("SetConsoleCtrlHandler() failed: %d\n", GetLastError());
		if (g_hCleanupEvent[0] != WSA_INVALID_EVENT) {
			WSACloseEvent(g_hCleanupEvent[0]);
			g_hCleanupEvent[0] = WSA_INVALID_EVENT;
		}
		WSACleanup();
		return(1);
	}

	for (i = 0; i < g_Options.nTotalThreads && !bInitError; i++) 
	{
		if (g_bEndClient)
			break;
		else if (CreateConnectedSocket(i)) 
		{
			nThreadNum[i] = i;
			g_ThreadInfo.hThread[i] = CreateThread(NULL, 0, EchoThread, (LPVOID)&nThreadNum[i], 0, &dwThreadId);
			if (g_ThreadInfo.hThread[i] == NULL) {
				myprintf("CreateThread(%d) failed: %d\n", i, GetLastError());
				bInitError = TRUE;
				break;
			}
		}
	}

	if (!bInitError) 
	{
		dwRet = WaitForMultipleObjects(g_Options.nTotalThreads, g_ThreadInfo.hThread, TRUE, INFINITE);
 		if (dwRet == WAIT_FAILED)
			myprintf("WaitForMultipleObject(): %d\n", GetLastError());
	}

	if (!GenerateConsoleCtrlEvent(CTRL_C_EVENT, 0)) {
		myprintf("GenerateConsoleCtrlEvent() failed: %d\n", GetLastError());
	};

	if (WSAWaitForMultipleEvents(1, g_hCleanupEvent, TRUE, WSA_INFINITE, FALSE) == WSA_WAIT_FAILED) {
		myprintf("WSAWaitForMultipleEvents() failed: %d\n", WSAGetLastError());
	};

	if (g_hCleanupEvent[0] != WSA_INVALID_EVENT) {
		WSACloseEvent(g_hCleanupEvent[0]);
		g_hCleanupEvent[0] = WSA_INVALID_EVENT;
	}

	WSACleanup();

	SetConsoleCtrlHandler(CtrlHandler, FALSE);
	SetConsoleCtrlHandler(NULL, FALSE);

	// 여기까지 래핑

	return(0);
}

static DWORD WINAPI EchoThread(LPVOID lpParameter) {

	char* inbuf = NULL;
	char* outbuf = NULL;
	int* pArg = (int*)lpParameter;
	int nThreadNum = *pArg;

	myprintf("Starting thread %d\n", nThreadNum);

	inbuf = (char*)xmalloc(g_Options.nBufSize);
	outbuf = (char*)xmalloc(g_Options.nBufSize);

	if ((inbuf) && (outbuf)) 
	{
		FillMemory(outbuf, g_Options.nBufSize, (BYTE)nThreadNum);

		while (TRUE) 
		{
			if (SendBuffer(nThreadNum, outbuf) && RecvBuffer(nThreadNum, inbuf)) 
			{
				if ((inbuf[0] == outbuf[0]) &&
					(inbuf[g_Options.nBufSize - 1] == outbuf[g_Options.nBufSize - 1])) {
					if (g_Options.bVerbose)
						myprintf("ack(%d)\n", nThreadNum);
				}
				else {
					myprintf("nak(%d) in[0]=%d, out[0]=%d in[%d]=%d out[%d]%d\n",
						nThreadNum,
						inbuf[0], outbuf[0],
						g_Options.nBufSize - 1, inbuf[g_Options.nBufSize - 1],
						g_Options.nBufSize - 1, outbuf[g_Options.nBufSize - 1]);
					break;
				}
			}
			else
				break;
		}
	}

	if (inbuf)
		xfree(inbuf);
	if (outbuf)
		xfree(outbuf);

	return(TRUE);
}

static BOOL CreateConnectedSocket(int nThreadNum) 
{
	BOOL bRet = TRUE;
	int nRet = 0;
	struct addrinfo hints = { 0 };
	struct addrinfo* addr_srv = NULL;

	hints.ai_flags = 0;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	if (getaddrinfo(g_Options.szHostname, g_Options.port, &hints, &addr_srv) != 0) 
	{
		myprintf("getaddrinfo() failed with error %d\n", WSAGetLastError());
		bRet = FALSE;
	}

	if (addr_srv == NULL) 
	{
		myprintf("getaddrinfo() failed to resolve/convert the interface\n");
		bRet = FALSE;
	}
	else {
		g_ThreadInfo.sd[nThreadNum] = socket(addr_srv->ai_family, addr_srv->ai_socktype, addr_srv->ai_protocol);
		if (g_ThreadInfo.sd[nThreadNum] == INVALID_SOCKET) 
		{
			myprintf("socket() failed: %d\n", WSAGetLastError());
			bRet = FALSE;
		}
	}

	if (bRet != FALSE) 
	{
		nRet = connect(g_ThreadInfo.sd[nThreadNum], addr_srv->ai_addr, (int)addr_srv->ai_addrlen);
		if (nRet == SOCKET_ERROR) 
		{
			myprintf("connect(thread %d) failed: %d\n", nThreadNum, WSAGetLastError());
			bRet = FALSE;
		}
		else
			myprintf("connected(thread %d)\n", nThreadNum);

		freeaddrinfo(addr_srv);
	}

	return(bRet);
}

static BOOL SendBuffer(int nThreadNum, char* outbuf) 
{
	// todo 왜 계속 send하는지 알아보기
	BOOL bRet = TRUE;
	const char* bufp = outbuf;
	int nTotalSend = 0;
	int nSend = 0;

	while (nTotalSend < g_Options.nBufSize) {
		nSend = send(g_ThreadInfo.sd[nThreadNum], bufp, g_Options.nBufSize - nTotalSend, 0);
		printf("sent!\n");
		if (nSend == SOCKET_ERROR) {
			myprintf("send(thread=%d) failed: %d\n", nThreadNum, WSAGetLastError());
			bRet = FALSE;
			break;
		}
		else if (nSend == 0) {
			myprintf("connection closed\n");
			bRet = FALSE;
			break;
		}
		else {
			// send
			nTotalSend += nSend;
			bufp += nSend;
			//std::cout << "send" << std::endl;
		}
	}

	return(bRet);
}

static BOOL RecvBuffer(int nThreadNum, char* inbuf) 
{
	BOOL bRet = TRUE;
	char* bufp = inbuf;
	int nTotalRecv = 0;
	int nRecv = 0;

	while (nTotalRecv < g_Options.nBufSize) 
	{
		nRecv = recv(g_ThreadInfo.sd[nThreadNum], bufp, g_Options.nBufSize - nTotalRecv, 0);
		if (nRecv == SOCKET_ERROR) {
			myprintf("recv(thread=%d) failed: %d\n", nThreadNum, WSAGetLastError());
			bRet = FALSE;
			break;
		}
		else if (nRecv == 0) {
			myprintf("connection closed\n");
			bRet = FALSE;
			break;
		}
		else {
			nTotalRecv += nRecv;
			bufp += nRecv;
		}
	}

	return(bRet);
}

static BOOL ValidOptions(char* argv[], int argc) 
{
	g_Options = default_options;
	HRESULT hRet;

	return(TRUE);
}

static VOID Usage(char* szProgramname, OPTIONS* pOptions) 
{
	myprintf("usage:\n%s [-b:#] [-e:#] [-n:host] [-t:#] [-v]\n",
		szProgramname);
	myprintf("%s -?\n", szProgramname);
	myprintf("  -?\t\tDisplay this help\n");
	myprintf("  -b:bufsize\tSize of send/recv buffer; in 1K increments (Def:%d)\n",
		pOptions->nBufSize);
	myprintf("  -e:port\tEndpoint number (port) to use (Def:%d)\n",
		pOptions->port);
	myprintf("  -n:host\tAct as the client and connect to 'host' (Def:%s)\n",
		pOptions->szHostname);
	myprintf("  -t:#\tNumber of threads to use\n");
	myprintf("  -v\t\tVerbose, print an ack when echo received and verified\n");
	return;
}

static BOOL WINAPI CtrlHandler(DWORD dwEvent) 
{
	int i = 0;
	DWORD dwRet = 0;

	switch (dwEvent) 
	{
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
	case CTRL_CLOSE_EVENT:

		myprintf("Closing handles and sockets\n");

		SetConsoleCtrlHandler(NULL, TRUE);

		g_bEndClient = TRUE;

		for (i = 0; i < g_Options.nTotalThreads; i++) 
		{
			if (g_ThreadInfo.sd[i] != INVALID_SOCKET) 
			{
				LINGER  lingerStruct;

				lingerStruct.l_onoff = 1;
				lingerStruct.l_linger = 0;
				setsockopt(g_ThreadInfo.sd[i], SOL_SOCKET, SO_LINGER,
					(char*)&lingerStruct, sizeof(lingerStruct));
				closesocket(g_ThreadInfo.sd[i]);
				g_ThreadInfo.sd[i] = INVALID_SOCKET;

				if (g_ThreadInfo.hThread[i] != INVALID_HANDLE_VALUE) {

					dwRet = WaitForSingleObject(g_ThreadInfo.hThread[i], INFINITE);
					if (dwRet == WAIT_FAILED)
						myprintf("WaitForSingleObject(): %d\n", GetLastError());

					CloseHandle(g_ThreadInfo.hThread[i]);
					g_ThreadInfo.hThread[i] = INVALID_HANDLE_VALUE;
				}
			}
		}

		break;

	default:
		return(FALSE);
	}

	WSASetEvent(g_hCleanupEvent[0]);

	return(TRUE);
}

static int myprintf(const char* lpFormat, ...) 
{
	int nLen = 0;
	std::cout << lpFormat << GetLastError() << std::endl;
	return nLen;
}