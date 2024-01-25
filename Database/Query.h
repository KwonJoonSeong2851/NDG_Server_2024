#pragma once
#include "../stdafx.h"
#include "ADODatabase.h"

#include "QueryRecord.h"
#include "QueryStatement.h"

/// <summary>
/// QueryStatement에서 쿼리문을 실행후 
/// QueryRecord의 DB응답을 담아 처리하기 위해 두 클래스를 묶어주는 클래스입니다.
/// </summary>
class Query
{
protected:
	QueryStatement* m_statement;
	QueryRecord m_record;

public:
	Query();
	virtual ~Query();

	void SetResult(recordPtr record);
	QueryRecord& GetResult();

	void SetStatement(QueryStatement* statement);
	QueryStatement* GetStatement();
};