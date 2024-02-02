#pragma once
#include "../stdafx.h"

typedef enum SERVER_STATUS
{
	SERVER_STOP,
	SERVER_INITIALZE,
	SERVER_READY,
};




/// <summary>
///  Server�� �������̽��� �����ϰ� Config ������ ������ �ε��ؼ� ������ �ʱ� �������ϴ� �߻�Ŭ�����Դϴ�.
/// </summary>
class Server
{
protected:
	char m_ip[16];
	int m_port;
	int m_workerThreadCount;

	SERVER_STATUS m_status;

	ContentsProcess* m_contentsProcess;

//	Protocol* protocol;

public:
	/// <summary> contetsProcess�� �޾� ������ ������ �����ϰ� Initialize�Լ��� ȣ���մϴ�. </summary>
	Server(ContentsProcess* contentsProcess);

	virtual ~Server();

	/// <summary>config ������ ������ �ε��ؼ� ��Ʈ��ũ ������ �մϴ�.</summary>
	virtual void Initialize(xml_t *config);

	virtual bool Run() = 0;

	SERVER_STATUS& GetStatus();

	/// <summary>��Ŷ�� �޾����� ó���� �մϴ�.</summary>
	void PutPackage(Package* package);

	//Protocol* GetProtocol();
};