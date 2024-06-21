#pragma once
#include "Types.h"
#include <windows.h>
#include "DBBind.h"

namespace SP
{
	
    class InsertGold : public DBBind<3,0>
    {
    public:
    	InsertGold(DBConnection& conn) : DBBind(conn, L"{CALL dbo.spInsertGold(?,?,?)}") { }
    	void In_Gold(int32& v) { BindParam(0, v); };
    	void In_Gold(int32&& v) { _gold = std::move(v); BindParam(0, _gold); };
    	template<int32 N> void In_Name(WCHAR(&v)[N]) { BindParam(1, v); };
    	template<int32 N> void In_Name(const WCHAR(&v)[N]) { BindParam(1, v); };
    	void In_Name(WCHAR* v, int32 count) { BindParam(1, v, count); };
    	void In_Name(const WCHAR* v, int32 count) { BindParam(1, v, count); };
    	void In_CreateDate(TIMESTAMP_STRUCT& v) { BindParam(2, v); };
    	void In_CreateDate(TIMESTAMP_STRUCT&& v) { _createDate = std::move(v); BindParam(2, _createDate); };

    private:
    	int32 _gold = {};
    	TIMESTAMP_STRUCT _createDate = {};
    };

    class GetGold : public DBBind<1,4>
    {
    public:
    	GetGold(DBConnection& conn) : DBBind(conn, L"{CALL dbo.spGetGold(?)}") { }
    	void In_Gold(int32& v) { BindParam(0, v); };
    	void In_Gold(int32&& v) { _gold = std::move(v); BindParam(0, _gold); };
    	void Out_Id(OUT int32& v) { BindCol(0, v); };
    	void Out_Gold(OUT int32& v) { BindCol(1, v); };
    	template<int32 N> void Out_Name(OUT WCHAR(&v)[N]) { BindCol(2, v); };
    	void Out_CreateDate(OUT TIMESTAMP_STRUCT& v) { BindCol(3, v); };

    private:
    	int32 _gold = {};
    };

    class DeleteGoldById : public DBBind<1,0>
    {
    public:
    	DeleteGoldById(DBConnection& conn) : DBBind(conn, L"{CALL dbo.spDeleteGoldById(?)}") { }
    	void In_Id(int32& v) { BindParam(0, v); };
    	void In_Id(int32&& v) { _id = std::move(v); BindParam(0, _id); };

    private:
    	int32 _id = {};
    };

    class DeleteAllGold : public DBBind<0,0>
    {
    public:
    	DeleteAllGold(DBConnection& conn) : DBBind(conn, L"{CALL dbo.spDeleteAllGold}") { }

    private:
    };


     
};