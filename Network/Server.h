#pragma once
#include "../stdafx.h"

typedef enum SERVER_STATUS
{
	SERVER_STOP,
	SERVER_INITIALZE,
	SERVER_READY,
};




/// <summary>
///  Server의 인터페이스를 제공하고 Config 파일의 설정을 로딩해서 서버의 초기 설정을하는 추상클래스입니다.
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
	/// <summary> contetsProcess를 받아 서버의 역할을 규정하고 Initialize함수를 호출합니다. </summary>
	Server(ContentsProcess* contentsProcess);

	virtual ~Server();

	/// <summary>config 파일의 설정을 로딩해서 네트워크 설정을 합니다.</summary>
	virtual void Initialize(xml_t *config);

	virtual bool Run() = 0;

	SERVER_STATUS& GetStatus();

	/// <summary>패킷을 받았을때 처리를 합니다.</summary>
	void PutPackage(Package* package);

	//Protocol* GetProtocol();
};