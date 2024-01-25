#pragma once
#include "../stdafx.h"
#include "ADODatabase.h"



/// <summary>
/// DB에 질의 후 받은 데이터 값을 관리하는 클래스입니다.
/// </summary>
class QueryRecord
{
	recordPtr m_record;
	int m_resultVal;

public:
	QueryRecord();
	~QueryRecord();

	void ErrorReport(_com_error& e);

	void operator=(QueryRecord& lvlue);
	void operator=(recordPtr& lvalue);

	recordPtr& GetResultRecord();

	bool Opened();
	void Close();
	void SetRecord(recordPtr record);
	bool IsEof();

	int ResultVal();
	void SetResultVal(int result);

	HRESULT MoveNext();
	HRESULT MovePrevious();
	HRESULT MoveFirst();
	HRESULT MoveLast();

	bool Get(const char* fieldName,  char* fieldValue);
	bool Get(const char* fieldName,  wchar_t* fieldValue);
	bool Get(const char* fieldName,  int& fieldValue);
	bool Get(const char* fieldName,  float& fieldValue);
	bool Get(const char* fieldName,  double& fieldValue);
	bool Get(const char* fieldName,  long& fieldValue);

};