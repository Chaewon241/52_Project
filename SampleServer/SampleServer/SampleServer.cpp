#include "pch.h"
#include "ThreadManager.h"
#include "Service.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "BufferWriter.h"
#include "ClientPacketHandler.h"
#include <tchar.h>
#include "Protocol.pb.h"
#include "DBConnectionPool.h"


int main()
{
	// 필요한 개수만큼 만들어주기
	// 나중에는 스레드 개수만큼 충분하게 만들어둠.
	//ASSERT_CRASH(GDBConnectionPool->Connect(1, L"Driver={SQL Server Native Client 11.0};Server=(localdb)\\MSSQLLocalDB;Database=ServerDb;Trusted_Connection=Yes;"));
	ASSERT_CRASH(GDBConnectionPool->Connect(1, L"Driver={ODBC Driver 17 for SQL Server};Server=(localdb)\\MSSQLLocalDB;Database=ServerDb;Trusted_Connection=yes;"));

	// Create Table
	// Gold라는 정보를 저장할 것이다.
	// dbo 골드라는게 있으면 날려라
	// 그리고 create해줘라
	// 어떤 유저가 골드를 얼마나 들고 있는지
	//
	{
		auto query = L"									\
			DROP TABLE IF EXISTS [dbo].[Gold];			\
			CREATE TABLE [dbo].[Gold]					\
			(											\
				[id] INT NOT NULL PRIMARY KEY IDENTITY, \
				[gold] INT NULL,						\
				[name] NVARCHAR(50) NULL,				\
				[createDate] DATETIME NULL				\
			);";

		// DBConnection을 빼와서 execute 해준다.
		DBConnection* dbConn = GDBConnectionPool->Pop();
		ASSERT_CRASH(dbConn->Execute(query));
		GDBConnectionPool->Push(dbConn);
	}

	// Add Data
	for (int32 i = 0; i < 3; i++)
	{
		DBConnection* dbConn = GDBConnectionPool->Pop();
		// 기존에 바인딩 된 정보 날림
		dbConn->Unbind();

		// 넘길 인자 바인딩
		int32 gold = 100;
		SQLLEN len = 0;

		WCHAR name[100] = L"루키스";
		SQLLEN namelen = 0;

		TIMESTAMP_STRUCT ts = { 2024, 6, 17 };
		SQLLEN tslen = 0;

		// 넘길 인자 바인딩
		ASSERT_CRASH(dbConn->BindParam(1, &gold, &len));
		ASSERT_CRASH(dbConn->BindParam(2, name, &namelen));
		ASSERT_CRASH(dbConn->BindParam(3, &ts, &tslen));

		// SQL 실행
		// 물음표는 우리가 넘겨줄 부분(인자 바인딩해줘서 넘겨줌)
		ASSERT_CRASH(dbConn->Execute(L"INSERT INTO [dbo].[Gold]([gold], [name], [createDate]) VALUES(?, ?, ?)"));

		GDBConnectionPool->Push(dbConn);
	}

	// Read
	{
		DBConnection* dbConn = GDBConnectionPool->Pop();
		// 기존에 바인딩 된 정보 날림
		dbConn->Unbind();

		int32 gold = 100;
		SQLLEN len = 0;
		// 넘길 인자 바인딩
		ASSERT_CRASH(dbConn->BindParam(1, &gold, &len));

		int32 outId = 0;
		SQLLEN outIdLen = 0;
		// 첫번째 컬럼 바인딩
		// outid에다가 결과 저장
		ASSERT_CRASH(dbConn->BindCol(1, &outId, &outIdLen));

		int32 outGold = 0;
		SQLLEN outGoldLen = 0;
		// 두번째 컬럼 바인딩
		// outid에다가 결과 저장
		ASSERT_CRASH(dbConn->BindCol(2, &outGold, &outGoldLen));

		WCHAR outName[100];
		SQLLEN outNameLen = 0;
		ASSERT_CRASH(dbConn->BindCol(3, outName, len32(outName), &outNameLen));

		TIMESTAMP_STRUCT outDate = {};
		SQLLEN outDateLen = 0;
		ASSERT_CRASH(dbConn->BindCol(4, &outDate, &outDateLen));

		// SQL 실행
		// SELECT문으로 시작
		// 골드가 100인 애들만 보여라
		ASSERT_CRASH(dbConn->Execute(L"SELECT id, gold, name, createDate FROM [dbo].[Gold] WHERE gold = (?)"));

		// 한국어 지원
		wcout.imbue(locale("kor"));
		// SELECT문이 뱉어줄 결과물은 3개이기 때문에 while문으로 fetch
		while (dbConn->Fetch())
		{
			wcout << "Id: " << outId << " Gold : " << outGold << " Name: " << outName << endl;
			wcout << "Date : " << outDate.year << "/" << outDate.month << "/" << outDate.day << endl;
		}

		GDBConnectionPool->Push(dbConn);
	}

	ClientPacketHandler::Init();

	ServerServiceRef service = MakeShared<ServerService>(
		NetAddress(L"127.0.0.1", 7777),
		MakeShared<IocpCore>(),
		MakeShared<GameSession>, // TODO : SessionManager 등
		100);

	ASSERT_CRASH(service->Start());

	for (int32 i = 0; i < 5; i++)
	{
		GThreadManager->Launch([=]()
			{
				while (true)
				{
					service->GetIocpCore()->Dispatch();
				}
			});
	}

	GThreadManager->Join();
}
