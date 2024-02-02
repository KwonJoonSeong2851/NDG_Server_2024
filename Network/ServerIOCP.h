#pragma once
#include "../stdafx.h"
//#include "../Network/Server.h"

#define MAX_IOCP_THREAD SIZE_64

class Thread;
class Server;
class ContentsProcess;

/// <summary>
/// Server Ŭ������ ��ӹ޾� IOCP ������ ���� ������ Ŭ�����Դϴ�.
/// </summary>
class IOCPServer : public Server, public Singleton<IOCPServer>
{
	SOCKET                      m_listenSocket;
	HANDLE                      m_iocp;
	Thread*                     m_acceptThread;
	std::array<Thread*, SIZE_64>     m_workerThread;

private:
	/// <summary>ListenSocket�� �����մϴ�. </summary>
	bool CreateListenSocket();

	/// <summary>Accept ��û�� �޽��ϴ�.</summary>
	static DWORD WINAPI AcceptThread(LPVOID serverPtr);

	/// <summary>Server ptr�� IOdata type�� ���� �ۼ��� ó���� �մϴ�.</summary>
	static DWORD WINAPI WorkerThread(LPVOID serverPtr);


public:
	IOCPServer(ContentsProcess* contentProcess);
	virtual ~IOCPServer();

	/// <summary>
	/// IOCPServer Ŭ���� �� ��ü���� ������ �����ϴ� �Լ��μ�, 
	/// CreateListenSocket() �Լ� ȣ�� �� Accept������, Woker�����带 �����մϴ�.
	/// </summary>
	bool Run();

	/// <summary>accept �� ȣ��Ǵ� �Լ��Դϴ�.</summary>
	void OnAccept(SOCKET accepter, SOCKADDR_IN addrInfo);

	SOCKET GetListenSocket();

	HANDLE GetIocp();

	
};