#pragma once
#include <sql.h>
#include <sqlext.h>

class DBConnection
{
public:
	// ������ ����
	bool			Connect(SQLHENV henv, const WCHAR* connectionString);
	void			Clear();

	// ������ �����ϴ� �Լ�
	bool			Execute(const WCHAR* query);
	// ������ �ϰ� ����� �޾ƿ��� �Լ�
	bool			Fetch();
	// �����Ͱ� ��� �ִ���
	int32			GetRowCount();
	void			Unbind();

public:
	// SQL ������ ������ �� ���ڸ� �Ѱ��� ���� �ִµ� statement�� ���ؼ� ��������Ѵ�.
	bool			BindParam(SQLUSMALLINT paramIndex, SQLSMALLINT cType, SQLSMALLINT sqlType, SQLULEN len, SQLPOINTER ptr, SQLLEN* index);
	bool			BindCol(SQLUSMALLINT columnIndex, SQLSMALLINT cType, SQLULEN len, SQLPOINTER value, SQLLEN* index);
	void			HandleError(SQLRETURN ret);

private:
	// �����ͺ��̽��� Ŀ�ؼ��� ����ϴ� �ڵ�
	SQLHDBC			_connection = SQL_NULL_HANDLE;
	// ���¸� �����ϴ� �ڵ�
	SQLHSTMT		_statement = SQL_NULL_HANDLE;
};

