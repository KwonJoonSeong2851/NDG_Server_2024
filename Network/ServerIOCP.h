#pragma once
#include "../stdafx.h"
//#include "../Network/Server.h"

#define MAX_IOCP_THREAD SIZE_64

class Thread;
class Server;
class ContentsProcess;

/// <summary>
/// Server 클래스를 상속받아 IOCP 서버로 세부 구현한 클래스입니다.
/// </summary>
class IOCPServer : public Server, public Singleton<IOCPServer>
{
	SOCKET                      m_listenSocket;
	HANDLE                      m_iocp;
	Thread*                     m_acceptThread;
	std::array<Thread*, SIZE_64>     m_workerThread;

private:
	/// <summary>ListenSocket을 생성합니다. </summary>
	bool CreateListenSocket();

	/// <summary>Accept 요청을 받습니다.</summary>
	static DWORD WINAPI AcceptThread(LPVOID serverPtr);

	/// <summary>Server ptr의 IOdata type에 따라 송수신 처리를 합니다.</summary>
	static DWORD WINAPI WorkerThread(LPVOID serverPtr);


public:
	IOCPServer(ContentsProcess* contentProcess);
	virtual ~IOCPServer();

	/// <summary>
	/// IOCPServer 클래스 내 전체적인 동작을 지시하는 함수로서, 
	/// CreateListenSocket() 함수 호출 및 Accept쓰레드, Woker쓰레드를 생성합니다.
	/// </summary>
	bool Run();

	/// <summary>accept 후 호출되는 함수입니다.</summary>
	void OnAccept(SOCKET accepter, SOCKADDR_IN addrInfo);

	SOCKET GetListenSocket();

	HANDLE GetIocp();

	
};