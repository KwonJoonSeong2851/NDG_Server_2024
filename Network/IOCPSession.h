#pragma once
#include "../stdafx.h"

class Session;
class SessionManager;
class Package;
class Packet;
class StreamBuffer;
class Protocol;

typedef enum
{
	IO_READ,
	IO_WRITE,
	IO_ERROR,
} IO_OPERATION;

static byte tcpPacketHead[9]
{
	(byte)251,
	(byte)0, //s
	(byte)0, //i
	(byte)0, //z
	(byte)0, //e 4byte
	(byte)0, //channel ID
	(byte)0, // deliver Mode 
	(byte)243,
	(byte)2, // EgMessageType
};

#define IO_DATA_MAX (2)

/// <summary>
/// Session에서 처리되는 Data만을 전담 처리하는 클래스입니다.
/// </summary>
class IoData
{
private:
	OVERLAPPED m_overlapped;
	IO_OPERATION m_ioType;
	size_t m_totalBytes;
	size_t m_currentBytes;

	std::array<char, SOCKET_BUF_SIZE> m_buffer;


public:
	IoData();
	~IoData() {};

	/// <summary>buffer 및 변수들을 초기화합니다. </summary>
	void Clear();

	/// <summary>현재 수신한 데이터가 전체 패킷크기와 동일한지 체크합니다.</summary>
	bool NeedMoreIo(size_t transferSize);

	/// <summary> m_buffer의 총 byte 크기를 설정합니다.</summary>
	int32_t SetupTotalBytes();

	size_t GetTotalBytes();

	/// <summary> m_ioType의 값을 반환합니다.. </summary>
	IO_OPERATION& GetType();

	/// <summary> m_ioType의 값을 설정합니다.</summary>
	void SetType(IO_OPERATION type);

	WSABUF GetWSABuf();

	char* GetData();

	/// <summary>해당 stream에 m_buffer의 데이터를 복사합니다. /summary>
	bool SetData(StreamBuffer& stream);

	LPWSAOVERLAPPED GetOverlapped();

};


class Session;
/// <summary>
///  Session 클래스를 상속받아 Client와 송수신하는 기능을 세부 구현한 클래스입니다.
/// </summary>
class IOCPSession : public Session
{

private:
	Protocol* protocol;

public:
	std::array<IoData, IO_DATA_MAX> m_ioData;

private:
	void Initialize();

	/// <summary>송수신시 오류를 검출합니다.</summary>
	void CheckErrorIO(DWORD ret);


	void Recv(WSABUF wsaBuf);
	bool IsRecving(size_t transferSize);

	void Send(WSABUF wsaBuf);

public:
	IOCPSession();

	void OnSend(size_t transferSize);
	void SendPacket(Packet* packet);

	Package* OnRecv(size_t transferSize);
	void RecvStandBy();
};