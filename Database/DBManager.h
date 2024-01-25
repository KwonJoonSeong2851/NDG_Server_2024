#pragma once
#include "../stdafx.h"
#include "ADODatabase.h"


/// <summary>
/// 여러 쓰레드에 처리할 쿼리들을 분산시켜 할당해주는 클래스입니다.
/// </summary>
class DBManager : public Singleton<DBManager>
{
	int m_workerCount;
	std::vector<Database*> m_dbPool;

	wstr_t m_serverName;
	wstr_t m_dbName;
	wstr_t m_login;
	wstr_t m_password;

	ThreadJobQueue<Query*>* m_queryPool;

public:
	DBManager();
	virtual ~DBManager();

	void Initialize(xml_t* config);

	size_t RunQueryCount();
	void PushQuery(Query* query);
	bool PopQuery(Query** query);

	void Run();

};