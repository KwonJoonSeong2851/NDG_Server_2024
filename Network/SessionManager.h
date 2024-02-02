#pragma once
#include "../stdafx.h"
#include "Session.h"


//최대 Sessoin 수
#define SESSION_CAPACITY (5000)

/// <summary>
///  전체 Session들을 관리하는 클래스입니다.
/// </summary>

class SessionManager : public Singleton<SessionManager>
{
	typedef std::list<Session*> SessionList;

	SessionList m_sessionList;
	int m_sessionCount;
	int m_maxConnection;
	Lock m_lock;
	oid_t m_idSeed;

	typedef std::function<void(SessionList* sessionList, wstr_t* arg)> cmdFunc;
	std::unordered_map<wstr_t, cmdFunc> m_serverCommand;

public:
	SessionManager();
	~SessionManager();

	oid_t CreateSessionId();

	bool AddSession(Session* session);
	bool CloseSession(Session* session);
	void ForceCloseSession(Session* session);


	std::list<Session*>& GetSessionList();
	Session* GetSession(oid_t id);

	Lock& GetLock();

	void RunCommand(wstr_t cmd);
	void CommandFucInitialize();
};