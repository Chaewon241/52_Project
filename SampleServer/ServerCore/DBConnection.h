#pragma once
#include <sql.h>
#include <sqlext.h>

class DBConnection
{
public:
	// 실제로 연결
	bool			Connect(SQLHENV henv, const WCHAR* connectionString);
	void			Clear();

	// 쿼리를 실행하는 함수
	bool			Execute(const WCHAR* query);
	// 실행을 하고 결과를 받아오는 함수
	bool			Fetch();
	// 데이터가 몇개가 있는지
	int32			GetRowCount();
	void			Unbind();

public:
	// SQL 쿼리를 실행할 때 인자를 넘겨줄 때가 있는데 statement를 통해서 보내줘야한다.
	bool			BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len, SQLPOINTER ptr, SQLLEN* index);
	bool			BindCol(SQLUSMALLINT columnIndex, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index);
	void			HandleError(SQLRETURN ret);

private:
	// 데이터베이스의 커넥션을 담당하는 핸들
	SQLHDBC			_connection = SQL_NULL_HANDLE;
	// 상태를 관리하는 핸들
	SQLHSTMT		_statement = SQL_NULL_HANDLE;
};

