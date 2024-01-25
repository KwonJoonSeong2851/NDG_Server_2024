#pragma once
#include "../stdafx.h"
#include "ADODatabase.h"

#include "QueryRecord.h"
#include "QueryStatement.h"

/// <summary>
/// QueryStatement���� �������� ������ 
/// QueryRecord�� DB������ ��� ó���ϱ� ���� �� Ŭ������ �����ִ� Ŭ�����Դϴ�.
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