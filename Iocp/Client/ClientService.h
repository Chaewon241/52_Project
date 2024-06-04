#pragma once

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

class ClientService
{

};